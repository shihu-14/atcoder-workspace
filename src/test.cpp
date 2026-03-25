#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <algorithm>
#include <optional>
#include <cstdint> // std::uint8_tのために必要

// --- 定数定義 ---
const float G = 1.0f;
const float DT = 0.016f; // 60FPS想定
const int STAR_PARTICLE_MAX_COUNT = 80000;
const int JET_PARTICLE_MAX_COUNT = 40000;

const unsigned int WINDOW_WIDTH = 1200;
const unsigned int WINDOW_HEIGHT = 1200;
const float STAR_PARTICLE_RADIUS = 0.2f;
const float JET_PARTICLE_RADIUS = 0.3f;

const float PARTICLE_LIFETIME = 3.0f;

// --- データ構造 ---
struct Particle {
    float x, y, z;
    float vx, vy, vz;
    float life;
    bool active;
    int type; // 0: Star Matter, 1: Jet/Gamma Ray
    sf::Color color;
};

class NeutronStarVFX {
private:
    std::vector<Particle> starParticles;
    std::vector<Particle> jetParticles;

    bool isMerged = false;
    int simulationFrame = 0;

    std::mt19937 rng;
    std::uniform_real_distribution<float> rand_pos_dist;

public:
    NeutronStarVFX() : 
        rng(std::random_device{}()),
        rand_pos_dist(-1.0f, 1.0f)
    {
        starParticles.reserve(STAR_PARTICLE_MAX_COUNT);
        jetParticles.reserve(JET_PARTICLE_MAX_COUNT);
        
        // 星1 (左側, オレンジ)
        spawnInitialStar(-5.0f, 0.0f, 0.0f, 0.0f, 0.7f, 0.0f, STAR_PARTICLE_MAX_COUNT / 2, sf::Color(255, 165, 0));
        // 星2 (右側, 明るいオレンジ)
        spawnInitialStar(5.0f, 0.0f, 0.0f, 0.0f, -0.7f, 0.0f, STAR_PARTICLE_MAX_COUNT / 2, sf::Color(255, 180, 0));
    }

    int getFrame() const { return simulationFrame; }

    void spawnInitialStar(float cx, float cy, float cz, float ivx, float ivy, float ivz, int count, sf::Color baseColor) {
        std::normal_distribution<float> dist_pos(0.0f, 0.8f);
        for(int i = 0; i < count; ++i) {
            Particle p;
            p.x = cx + dist_pos(rng);
            p.y = cy + dist_pos(rng);
            p.z = cz + dist_pos(rng);
            p.vx = ivx; p.vy = ivy; p.vz = ivz;
            p.life = PARTICLE_LIFETIME;
            p.active = true;
            p.type = 0;
            
            // SFML 3.0対応: sf::Uint8 -> std::uint8_t
            p.color = baseColor;
            p.color.r = static_cast<std::uint8_t>(std::min(255, baseColor.r + static_cast<int>(rand_pos_dist(rng) * 30)));
            p.color.g = static_cast<std::uint8_t>(std::min(255, baseColor.g + static_cast<int>(rand_pos_dist(rng) * 30)));
            p.color.b = static_cast<std::uint8_t>(std::min(255, baseColor.b + static_cast<int>(rand_pos_dist(rng) * 30)));

            starParticles.push_back(p);
        }
    }

    void updateParticlePhysics(std::vector<Particle>& particles_vec) {
        for (auto& p : particles_vec) {
            if (!p.active) continue;

            p.life -= DT;
            if (p.life <= 0) {
                p.active = false;
                continue;
            }

            float distSq = p.x*p.x + p.y*p.y + p.z*p.z;
            float dist = std::sqrt(distSq);
            if (dist < 0.1f) dist = 0.1f;

            float gravitational_force = 10.0f / (distSq + 1.0f);
            float ax = -gravitational_force * (p.x / dist);
            float ay = -gravitational_force * (p.y / dist);
            float az = -gravitational_force * (p.z / dist);

            float rotational_force = 5.0f / (dist + 0.5f);
            ax += -p.y * rotational_force;
            ay += p.x * rotational_force;

            if (p.type == 0) {
                p.vx *= 0.995f; p.vy *= 0.995f; p.vz *= 0.995f;
            } else {
                ax = 0; ay = 0; az = 0;
                p.vx *= 1.02f; p.vy *= 1.02f; p.vz *= 1.02f;
            }

            p.vx += ax * DT; p.vy += ay * DT; p.vz += az * DT;
            p.x += p.vx * DT; p.y += p.vy * DT; p.z += p.vz * DT;
        }

        particles_vec.erase(
            std::remove_if(particles_vec.begin(), particles_vec.end(), [](const Particle& p){ return !p.active; }),
            particles_vec.end()
        );
    }

    void update() {
        simulationFrame++;
        updateParticlePhysics(starParticles);
        updateParticlePhysics(jetParticles);

        if (!isMerged && simulationFrame > 150) {
            isMerged = true;
            std::cout << "[Event] Merger Triggered!" << std::endl;
        }
        
        if (isMerged && simulationFrame % 3 == 0 && jetParticles.size() < JET_PARTICLE_MAX_COUNT) {
            spawnJet();
        }
    }

    void spawnJet() {
        std::normal_distribution<float> spread_pos(0.0f, 0.1f);
        std::normal_distribution<float> spread_vel(0.0f, 0.05f);
        float jetSpeed = 40.0f;

        for(int i=0; i<8; ++i) {
            // 上方向
            Particle p_up;
            p_up.x = spread_pos(rng); p_up.y = spread_pos(rng); p_up.z = 0.0f;
            p_up.vx = spread_vel(rng); p_up.vy = spread_vel(rng); p_up.vz = jetSpeed;
            p_up.life = PARTICLE_LIFETIME * 1.5f;
            p_up.active = true; p_up.type = 1;
            p_up.color = sf::Color::White;
            jetParticles.push_back(p_up);

            // 下方向
            Particle p_down;
            p_down.x = spread_pos(rng); p_down.y = spread_pos(rng); p_down.z = 0.0f;
            p_down.vx = spread_vel(rng); p_down.vy = spread_vel(rng); p_down.vz = -jetSpeed;
            p_down.life = PARTICLE_LIFETIME * 1.5f;
            p_down.active = true; p_down.type = 1;
            p_down.color = sf::Color::White;
            jetParticles.push_back(p_down);
        }
    }

    const std::vector<Particle>& getStarParticles() const { return starParticles; }
    const std::vector<Particle>& getJetParticles() const { return jetParticles; }
    bool isSimulationRunning() const { return simulationFrame < 4000; }
};

int main() {
    sf::ContextSettings settings;
    // SFML 3.0対応: antialiasingLevel -> antiAliasingLevel
    settings.antiAliasingLevel = 8; 

    // SFML 3.0対応: コンストラクタの引数変更
    // sf::Style::Default は廃止/変更され、第3引数は sf::State になりました
    sf::RenderWindow window(
        sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), 
        "Neutron Star Merger (SFML 3.0 Final)", 
        sf::State::Windowed, // ここが重要
        settings
    );
    
    window.setFramerateLimit(60);

    NeutronStarVFX simulation;

    sf::CircleShape particleShape(1.0f); 

    // SFML 3.0対応: Vector2fを使用 ({x, y}, {w, h})
    sf::View view(sf::FloatRect({-15.0f, -15.0f}, {30.0f, 30.0f})); 
    window.setView(view);

    // SFML 3.0対応: sf::BlendAdd を使用
    // 波括弧 {} 初期化を使用して関数宣言と誤認されるのを防ぐ
    sf::RenderStates states{sf::BlendAdd};

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        if (!simulation.isSimulationRunning()) {
            window.close();
            break;
        }

        simulation.update();

        window.clear(sf::Color::Black);

        // 星の描画
        for (const auto& p : simulation.getStarParticles()) {
            if (!p.active) continue;
            
            sf::Color drawColor = p.color;
            // SFML 3.0対応: std::uint8_t キャスト
            drawColor.a = static_cast<std::uint8_t>(std::max(0.0f, std::min(255.0f, p.life / PARTICLE_LIFETIME * 255.0f)));
            particleShape.setFillColor(drawColor);
            
            float currentRadius = STAR_PARTICLE_RADIUS * (p.life / PARTICLE_LIFETIME);
            particleShape.setRadius(currentRadius);
            // SFML 3.0対応: 位置指定にVector2fを使用
            particleShape.setPosition({p.x - currentRadius, p.y - currentRadius});
            
            window.draw(particleShape, states);
        }

        // ジェットの描画
        for (const auto& p : simulation.getJetParticles()) {
            if (!p.active) continue;

            sf::Color drawColor = p.color;
            drawColor.a = static_cast<std::uint8_t>(std::max(0.0f, std::min(255.0f, p.life / (PARTICLE_LIFETIME * 1.5f) * 255.0f * 1.5f)));
            particleShape.setFillColor(drawColor);

            float currentRadius = JET_PARTICLE_RADIUS * (p.life / (PARTICLE_LIFETIME * 1.5f));
            particleShape.setRadius(currentRadius);
            particleShape.setPosition({p.x - currentRadius, p.y - currentRadius});
            
            window.draw(particleShape, states);
        }

        window.display();
    }

    return 0;
}