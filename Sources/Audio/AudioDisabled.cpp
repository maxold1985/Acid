// Silent audio backend: preserves the public API without linking OpenAL.
#include "Audio.hpp"
#include "Sound.hpp"
#include "SoundBuffer.hpp"
#include "Resources/Resources.hpp"
#include "Maths/Transform.hpp"
#include "Scenes/Entity.hpp"

namespace acid {
struct Audio::_intern {};
Audio::Audio() : impl(std::make_unique<_intern>()) {}
Audio::~Audio() = default;
void Audio::Update() {}
std::string Audio::StringifyResultAl(int32_t) { return "Audio disabled"; }
void Audio::CheckAl(int32_t) {}
float Audio::GetGain(Type type) const {
    auto it = gains.find(type);
    return it == gains.end() ? 1.0f : it->second;
}
void Audio::SetGain(Type type, float volume) {
    gains[type] = volume;
    onGain(type, volume);
}
Sound::Sound(const std::string &filename, const Audio::Type &type, bool begin, bool loop, float gain, float pitch)
    : type(type), gain(gain), pitch(pitch) {
    (void)filename; (void)begin; (void)loop;
}
Sound::~Sound() = default;
void Sound::Start() {}
void Sound::Update() {}
void Sound::Play(bool) {}
void Sound::Pause() {}
void Sound::Resume() {}
void Sound::Stop() {}
bool Sound::IsPlaying() const { return false; }
void Sound::SetPosition(const Vector3f &value) { position = value; }
void Sound::SetDirection(const Vector3f &value) { direction = value; }
void Sound::SetVelocity(const Vector3f &value) { velocity = value; }
void Sound::SetGain(float value) { gain = value; }
void Sound::SetPitch(float value) { pitch = value; }
const Node &operator>>(const Node &node, Sound &sound) {
    node["buffer"].Get(sound.buffer);
    node["type"].Get(sound.type);
    node["gain"].Get(sound.gain);
    node["pitch"].Get(sound.pitch);
    return node;
}
Node &operator<<(Node &node, const Sound &sound) {
    node["buffer"].Set(sound.buffer);
    node["type"].Set(sound.type);
    node["gain"].Set(sound.gain);
    node["pitch"].Set(sound.pitch);
    return node;
}
std::shared_ptr<SoundBuffer> SoundBuffer::Create(const Node &node) {
    if (auto resource = Resources::Get()->Find<SoundBuffer>(node)) return resource;
    auto result = std::make_shared<SoundBuffer>("");
    Resources::Get()->Add(node, std::dynamic_pointer_cast<Resource>(result));
    node >> *result;
    return result;
}
std::shared_ptr<SoundBuffer> SoundBuffer::Create(const std::filesystem::path &filename) {
    SoundBuffer temp(filename, false);
    Node node;
    node << temp;
    return Create(node);
}
SoundBuffer::SoundBuffer(std::filesystem::path filename, bool) : filename(std::move(filename)) {}
SoundBuffer::~SoundBuffer() = default;
void SoundBuffer::SetBuffer(uint32_t value) { buffer = value; }
void SoundBuffer::Load() {}
const Node &operator>>(const Node &node, SoundBuffer &value) {
    node["filename"].Get(value.filename);
    return node;
}
Node &operator<<(Node &node, const SoundBuffer &value) {
    node["filename"].Set(value.filename);
    return node;
}
}
