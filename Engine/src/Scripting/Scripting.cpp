#include "Scripting/Scripting.h"
#include "Architecture/Application.h"
#include "LayerSystem/Layer.h"

namespace IonixEngine {
	Scripting* Scripting::s_Instance = nullptr;

	Scripting& Scripting::Get() {
		if (!s_Instance) {
			s_Instance = new Scripting();
		}
		return *s_Instance;
	}
	void Scripting::Init() {
		std::cout << "Lua is now being initialized." << std::endl;

		// Open all libraries first
		m_LuaState.open_libraries(
			sol::lib::base,
			sol::lib::string,
			sol::lib::math,
			sol::lib::table,
			sol::lib::io
		);

		// Register engine systems
		RegisterEngineBindings();

		std::cout << "Lua has been initialised successfully." << std::endl;
	}



	void Scripting::RegisterEngineBindings()
	{
		RegisterWindowBindings();
		RegisterInputBindings();
		RegisterMafsBindings();
		RegisterAudioBindings();
		RegisterGraphicsBindings();
		RegisterEntityBindings();
		RegisterUIBindings();
	}

	void Scripting::ExecuteScript(const std::string& scriptName)
	{
		m_LuaState.script_file(scriptName);
	}

	void Scripting::CallHook(const std::string& hookName)
	{
		sol::function hook = m_LuaState[hookName];
		if (hook.valid()) {
			try {
				hook();
			}
			catch (const std::exception& e) {
				std::cerr << "Error calling hook '" << hookName << "': " << e.what() << '\n';
			}
		}
		else {
			std::cerr << "Hook '" << hookName << "' is invalid\n";
		}
	}

	void Scripting::RegisterWindowBindings()
	{
		auto getWindowTitle = []()->std::string {

			return Application::Get().GetWindow().m_Data.Title;
			};
		auto getWindowWidth = []() -> std::int32_t {
			return Application::Get().GetWindow().m_Data.Width;
			};
		auto getWindowHeight = []()-> std::int32_t {
			return Application::Get().GetWindow().m_Data.Height;
			};
		m_LuaState["Window"] = m_LuaState.create_table_with(
			"get_title", getWindowTitle,
			"get_width", getWindowWidth,
			"get_height", getWindowHeight
		);

	}
	void Scripting::RegisterInputBindings()
	{
		auto getKeyUp = [](int code) -> bool {
			return Application::Get().layerInput->m_Input->IsKeyUp(static_cast<SDL_Scancode>(code));
			};
		auto getKeyDown = [](int code) -> bool {
			return Application::Get().layerInput->m_Input->IsKeyDown(static_cast<SDL_Scancode>(code));
			};
		auto getKeyHeld = [](int code) -> bool {
			return Application::Get().layerInput->m_Input->IsKeyHeld(static_cast<SDL_Scancode>(code));
			};


		m_LuaState["Keys"] = m_LuaState.create_table_with(
			"ionix_a", SDL_SCANCODE_A,
			"ionix_b", SDL_SCANCODE_B,
			"ionix_c", SDL_SCANCODE_C,
			"ionix_d", SDL_SCANCODE_D,
			"ionix_e", SDL_SCANCODE_E,
			"ionix_f", SDL_SCANCODE_F,
			"ionix_g", SDL_SCANCODE_G,
			"ionix_h", SDL_SCANCODE_H,
			"ionix_i", SDL_SCANCODE_I,
			"ionix_j", SDL_SCANCODE_J,
			"ionix_k", SDL_SCANCODE_K,
			"ionix_l", SDL_SCANCODE_L,
			"ionix_m", SDL_SCANCODE_M,
			"ionix_n", SDL_SCANCODE_N,
			"ionix_o", SDL_SCANCODE_O,
			"ionix_p", SDL_SCANCODE_P,
			"ionix_q", SDL_SCANCODE_Q,
			"ionix_r", SDL_SCANCODE_R,
			"ionix_s", SDL_SCANCODE_S,
			"ionix_t", SDL_SCANCODE_T,
			"ionix_u", SDL_SCANCODE_U,
			"ionix_v", SDL_SCANCODE_V,
			"ionix_w", SDL_SCANCODE_W,
			"ionix_x", SDL_SCANCODE_X,
			"ionix_y", SDL_SCANCODE_Y,
			"ionix_z", SDL_SCANCODE_Z,
			"ionix_1", SDL_SCANCODE_1,
			"ionix_2", SDL_SCANCODE_2,
			"ionix_3", SDL_SCANCODE_3,
			"ionix_4", SDL_SCANCODE_4,
			"ionix_5", SDL_SCANCODE_5,
			"ionix_6", SDL_SCANCODE_6,
			"ionix_7", SDL_SCANCODE_7,
			"ionix_8", SDL_SCANCODE_8,
			"ionix_9", SDL_SCANCODE_9,
			"ionix_0", SDL_SCANCODE_0,
			"ionix_return", SDL_SCANCODE_RETURN,
			"ionix_escape", SDL_SCANCODE_ESCAPE,
			"ionix_backspace", SDL_SCANCODE_BACKSPACE,
			"ionix_tab", SDL_SCANCODE_TAB,
			"ionix_space", SDL_SCANCODE_SPACE,
			"ionix_minus", SDL_SCANCODE_MINUS,
			"ionix_equals", SDL_SCANCODE_EQUALS,
			"ionix_leftbracket", SDL_SCANCODE_LEFTBRACKET,
			"ionix_rightbracket", SDL_SCANCODE_RIGHTBRACKET,
			"ionix_backslash", SDL_SCANCODE_BACKSLASH,
			"ionix_lctrl", SDL_SCANCODE_LCTRL,
			"ionix_lshift", SDL_SCANCODE_LSHIFT,
			"ionix_lalt", SDL_SCANCODE_LALT,
			"ionix_lgui", SDL_SCANCODE_LGUI,
			"ionix_rctrl", SDL_SCANCODE_RCTRL,
			"ionix_rshift", SDL_SCANCODE_RSHIFT,
			"ionix_ralt", SDL_SCANCODE_RALT,
			"ionix_rgui", SDL_SCANCODE_RGUI
		);

		m_LuaState["Input"] = m_LuaState.create_table_with(
			"get_key_up", getKeyUp,
			"get_key_down", getKeyDown,
			"get_key_held", getKeyHeld
		);
	}
	void Scripting::RegisterMafsBindings()
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

		auto vector2 = [](float x, float y) -> Maf::mafVector2<float> {
			return Maf::mafVector2(x, y);
			};

		auto vector2x = [](Maf::mafVector2<float> vec2) -> float {
			return vec2.x;
			};

		auto vector2y = [](Maf::mafVector2<float> vec2) -> float {
			return vec2.y;
			};

		auto vector3 = [](float x, float y, float z) -> Maf::mafVector3<float> {
			return Maf::mafVector3(x, y, z);
			};

		auto vector3x = [](Maf::mafVector3<float> vec3) -> float {
			return vec3.x;
			};

		auto vector3y = [](Maf::mafVector3<float> vec3) -> float {
			return vec3.y;
			};

		auto vector3z = [](Maf::mafVector3<float> vec3) -> float {
			return vec3.z;
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

		m_LuaState["Mafs"] = m_LuaState.create_table_with(
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
			"vector2", vector2,
			"vector2_x", vector2x,
			"vector2_y", vector2y,
			"vector3", vector3,
			"vector3_x", vector3x,
			"vector3_y", vector3y,
			"vector3_z", vector3z,
			"mod", mod,
			"div", div,
			"sqr_magnitude_vector2", SqrMagnitudeVector2,
			"sqr_magnitude_vector3", SqrMagnitudeVector3,
			"sin", sin,
			"cos", cos,
			"tan", tan,
			"pi", pi,
			"rad_2_deg", rad2deg,
			"deg_2_rad", deg2rad
		);
	}
	void Scripting::RegisterAudioBindings()
	{
		AudioScripting::Get().Init(m_LuaState);


	}

	void Scripting::RegisterGraphicsBindings()
	{

		auto texture = [](std::string filePath, std::string alias)
			{
				TextureManager::Get().AddTexture(filePath, alias);
			};

		m_LuaState["Texture"] = m_LuaState.create_table_with(
			"add_texture", texture
		);
	}

	void Scripting::RegisterEntityBindings()
	{
		auto entity = []() -> Entity* {
			EntityID entityID = Application::Get().layerScene->GetScene()->CreateEntity();
			return Application::Get().layerScene->GetScene()->GetEntityFromID(entityID);
			};

		auto getEntityPos = [](Entity* entity) -> Vec2 {
			return entity->position;
			};

		auto setEntityPos = [](Entity* entity, float x, float y) {
			if (entity == nullptr) return;
			entity->position = Vec2{ x, y };
			};

		auto addSpriteComponent = [](Entity* entity, string alias, int zedOrder) {
			entity->AddComponent(new SpriteComponent(entity, alias, zedOrder));
			};

		m_LuaState["Entity"] = m_LuaState.create_table_with(
			"create_entity", entity,
			"get_entity_pos", getEntityPos,
			"set_entity_pos", setEntityPos,
			"add_sprite_component", addSpriteComponent
		);
	}

	void Scripting::RegisterUIBindings()
	{
    UI-Main
		auto drawLabel = [this](const char* text, int xsize, int ysize, int xpos, int ypos, const char* font) {
			Application::Get().layerUI->m_UI->DrawLabel((char*)text, xsize, ysize, xpos, ypos, "");
			};

		auto drawButton = [](const char* text, int xsize, int ysize, int xpos, int ypos) -> bool {
			return Application::Get().layerUI->m_UI->DrawButton((char*)text, xsize, ysize, xpos, ypos);
			};

		auto drawSlider = [](const char* text, float i, int xsize, int ysize, int xpos, int ypos, int minval, int maxval) -> float {
			return Application::Get().layerUI->m_UI->DrawSlider((char*)text, i, xsize, ysize, xpos, ypos, minval, maxval);
			};

		m_LuaState["UI"] = m_LuaState.create_table_with(
			"draw_label", drawLabel,
			"draw_button", drawButton,
			"draw_slider", drawSlider
		);



		// print (Window.get_title())
	}
}
