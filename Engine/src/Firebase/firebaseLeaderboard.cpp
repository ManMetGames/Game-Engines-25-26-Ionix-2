#include "Architecture/Application.h"
#include "firebaseLeaderboard.h"
#include <fstream>

namespace IonixEngine
{
	static firebase::App* g_app = nullptr;
	static firebase::database::Database* g_db = nullptr;

}