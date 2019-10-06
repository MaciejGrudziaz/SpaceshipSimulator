//#pragma once
//
//#include "particlesystem.h"
//
//struct ParticleSystemThread
//{
//	std::list<ParticleSystemPtr> particleSystems;
//	bool closeThreadFlag;
//
//	void operator()() {
//		while (!closeThreadFlag)
//		{
//			for (auto particleSystem : particleSystems)
//			{
//				particleSystem->processParticles();
//			}
//		}
//	}
//};
//
//
//class ParticleSystemManager
//{
//public:
//	ParticleSystemManager(int threadCount);
//
//	void registerParticleSystem(ParticleSystemPtr particleSystem);
//
//	void launch();
//	void invalidate();
//
//private:
//	int threadCount;
//	std::vector<ParticleSystemPtr> particleSystems;
//
//	std::list<std::shared_ptr<ParticleSystemThread> > particleThreadObjects;
//	std::list<std::shared_ptr<std::thread> > threads;
//
//};
