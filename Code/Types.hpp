#pragma once

using Void = std::uint32_t;
using Any = std::uint32_t;
using Hash = std::uint32_t;
using Entity = std::int32_t;
using Player = std::int32_t;
using FireId = std::int32_t;
using Ped = Entity;
using Vehicle = Entity;
using Cam = std::int32_t;
using CarGenerator = std::int32_t;
using Group = std::int32_t;
using Train = std::int32_t;
using Object = Entity;
using Pickup = Object;
using Weapon = std::int32_t;
using Interior = std::int32_t;
using Blip = std::int32_t;
using Texture = std::int32_t;
using TextureDict = std::int32_t;
using CoverPoint = std::int32_t;
using Camera = std::int32_t;
using TaskSequence = std::int32_t;
using ColourIndex = std::int32_t;
using Sphere = std::int32_t;
using ScrHandle = std::int32_t;

struct Vector2
{
	float x{};
	float y{};
};

struct Vector3_t
{
public:
	float x{};
public:
	float y{};
public:
	float z{};
private:
};

struct Vector3
{
public:
	float x{};
private:
	char paddingX[4];
public:
	float y{};
private:
	char paddingY[4];
public:
	float z{};
private:
	char paddingZ[4];
private:
};

struct Color
{
	std::uint8_t r{ 255 };
	std::uint8_t g{ 255 };
	std::uint8_t b{ 255 };
	std::uint8_t a{ 255 };
};


