#include "Scripting/Mafs/MafsScripting.h"
#include "Architecture/Application.h"
namespace IonixEngine
{
	MafsScripting* MafsScripting::s_Instance = nullptr;

	MafsScripting& MafsScripting::Get() {
		if (!s_Instance)
			s_Instance = new MafsScripting();
		return *s_Instance;
	}

	void MafsScripting::Init(sol::state& lua)
	{
		auto clamp = [](float x, float min, float max) -> float {
			return Maf::mafClamp(x, min, max);
			};
		auto log = [](float x) -> float {
			return Maf::Log(x);
			};
		auto log10 = [](float value) -> float {
			return Maf::Log10(value);
			};
		auto logCustom = [](float x, float base) -> float {
			return Maf::Log(x, base);
			};

		auto sqrt = [](float x) -> float {
			return Maf::mafSqrt(x);
			};

		auto lerp = [](float x, float y, float t) -> float {
			return Maf::mafLerp(x, y, t);
			};

		auto vector2 = [](float x, float y) -> b2Vec2* {
			b2Vec2* vector = new b2Vec2(x, y);
			return vector;
			};

		auto mod = [](float x, float y) -> float {
			return Maf::mafMod(x, y);
			};

		auto div = [](float x, float y) -> float {
			return Maf::mafDiv(x, y);
			};

		auto SqrMagnitudeVector2 = [](Maf::mafVector2<float> v) -> float {
			return Maf::mafSqrMagnitudeVec2(v);
			};

		auto SqrMagnitudeVector3 = [](Maf::mafVector3<float> v) -> float {
			return Maf::mafSqrMagnitudeVec3(v);
			};

		auto abs = [](float x) -> float {
			return Maf::mafAbs(x);
			};

		auto min = [](float x, float minVal) -> float {
			return Maf::mafMin(x, minVal);
			};

		auto max = [](float x, float maxVal) -> float {
			return Maf::mafMax(x, maxVal);
			};

		auto round = [](float x) -> float {
			return Maf::mafRound(x);
			};

		auto sin = [](float x) -> float {
			return Maf::mafSin(x);
			};

		auto cos = [](float x) -> float {
			return Maf::mafCos(x);
			};

		auto tan = [](float x) -> float {
			return Maf::mafTan(x);
			};

		auto pi = []() -> double {
			return Maf::mafPI();
			};

		auto rad2deg = [](float radians) -> float {
			return Maf::Rad2Deg(radians);
			};

		auto deg2rad = [](float degrees) -> float {
			return Maf::Deg2Rad(degrees);
			};

		auto deltatime = []() -> float {
			return Application::Get().deltaTime;
			};

		auto time = []() -> float {
			return Application::Get().time;
			};

		lua["Mafs"] = lua.create_table_with(
			"clamp", clamp,
			"abs", abs,
			"min", min,
			"max", max,
			"round", round,
			"log", log,
			"log_10", log10,
			"log_custom", logCustom,
			"square_root", sqrt,
			"lerp", lerp,
			"vec_2", vector2,
			"mod", mod,
			"div", div,
			"sqr_magnitude_vector2", SqrMagnitudeVector2,
			"sqr_magnitude_vector3", SqrMagnitudeVector3,
			"sin", sin,
			"cos", cos,
			"tan", tan,
			"pi", pi,
			"delta_time", deltatime,
			"time", time,
			"rad_2_deg", rad2deg,
			"deg_2_rad", deg2rad
		);
	}
}


