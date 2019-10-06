//#include "particlesystemmanager.h"
//
//ParticleSystemManager::ParticleSystemManager(int threadCount)
//	: threadCount(threadCount)
//{}
//
//void ParticleSystemManager::registerParticleSystem(ParticleSystemPtr particleSystem)
//{
//	particleSystems.push_back(particleSystem);
//}
//
//void ParticleSystemManager::launch()
//{
//	if (particleSystems.size() < threadCount)
//	{
//		for (auto particleSys : particleSystems)
//		{
//			std::shared_ptr<ParticleSystemThread> particleThreadObj = std::make_shared<ParticleSystemThread>();
//			particleThreadObj->closeThreadFlag = false;
//			particleThreadObj->particleSystems.push_back(particleSys);
//
//			particleThreadObjects.push_back(particleThreadObj);
//
//			std::shared_ptr<std::thread> particleThread = std::make_shared<std::thread>(particleThreadObj);
//			threads.push_back(particleThread);
//		}
//	}
//	else {
//
//	}
//}
//
//void ParticleSystemManager::invalidate()
//{
//	
//}
