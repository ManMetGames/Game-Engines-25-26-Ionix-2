// Used by Client.cpp

#pragma once
#include "Architecture/Application.h"
#include "Architecture/main.h"
#include "Architecture/Macros.h"
#include "LayerSystem/Layer.h"
#include "LayerSystem/LayerStack.h"
#include "imgui.h"
namespace MafTrig {
	template <typename T>
	inline T mafSin(T x) {
		return sin(x);
	}
	template <typename T>
	inline T mafCos(T x) {
		return cos(x);
	}
	template <typename T> 
	inline T mafTan(T x) {
		return tan(x);
	}
	double mafPI(){
		return 3.14159265358979323846264338327950288;
	}
}