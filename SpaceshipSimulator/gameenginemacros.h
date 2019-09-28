#pragma once

#define GAME_INIT(engine)		template<> \
								void MG::gameResourcesInit<GameEngine>(GameEngine& engine) \
								{ \
									engine.registerResources(std::make_shared<GameResources<GameEngine> >()); \
								} \
								template<> \
								void MG::initialize<GameEngine>(GameEngine& engine) \

#define GAME_PROCESS(engine)	template<> \
								void MG::process<GameEngine>(GameEngine& engine) \

#define GAME_END(engine)		template<> \
								void MG::end<GameEngine>(GameEngine& engine) \

#define GAME_RESOURCES			typedef std::shared_ptr<GameResources<GameEngine> > GameResourcesPtr; \
								template<> \
								class GameResources<GameEngine> \
