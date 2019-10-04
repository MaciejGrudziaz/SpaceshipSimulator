#include "collisiondetectionalgorithm.h"

//sprawdzanie kolizji pomi�dzy dwoma modelami
//sprawdzenie nast�puje pomi�dzy g��wn� bry�� kolizji modelu pierwszego ['model1'] ,a wszystkimi mo�liwymi bry�ami kolizji modelu drugiego ['model2']
bool CollisionDetection::CheckCollision(const Hitbox& model1, const Hitbox& model2) {
	glm::vec3 model1_midPt(0.0f), model2_midPt(0.0f);
	glm::vec3 model1_min, model1_max, model2_min, model2_max;
	float model1_R, model2_R;

	model1_min = model1_max = model1.basicVertices[0];
	model2_min = model2_max = model2.transformVertices[0];

	for (int i = 0; i < 8; ++i) {
		model1_midPt += model1.transformVertices[i];
		model2_midPt += model2.transformVertices[i];
		for (int j = 0; j < 3; ++j) {
			if (model1.transformVertices[i][j] < model1_min[j]) model1_min[j] = model1.transformVertices[i][j];
			if (model1.transformVertices[i][j] > model1_max[j]) model1_max[j] = model1.transformVertices[i][j];
			if (model2.transformVertices[i][j] < model2_min[j]) model2_min[j] = model2.transformVertices[i][j];
			if (model2.transformVertices[i][j] > model2_max[j]) model2_max[j] = model2.transformVertices[i][j];
		}
	}
	model1_midPt /= 8.0f;
	model2_midPt /= 8.0f;

	model1_R = glm::length(model1_max - model1_min) / 2.0f;
	model2_R = glm::length(model2_max - model2_min) / 2.0f;

	if (glm::length(model1_midPt - model2_midPt) < (model1_R + model2_R)) {
		std::vector<glm::vec3> hitbox1, hitbox2;

		for (int i = 0; i < 8; ++i) {
			hitbox1.push_back(model1.transformVertices[i]);
			hitbox2.push_back(model2.transformVertices[i]);
		}

		return GJK(hitbox1, hitbox2);
	}
	else return false;
}

bool CollisionDetection::CheckCollision(HitboxObject& model, const glm::vec3& point)
{
	glm::vec3 pos = model.getParentTransform() * glm::vec4(model.getTransform().getPosition(),1.0f);
	float radius = model.getCollisionRadius();
	bool collisionOccured;

	if (glm::distance(pos, point) <= radius)
	{
		std::array<SurfacePtr, 6> hitboxSurfaces;

		createSurfaces(hitboxSurfaces, *(model.getHitbox()));

		collisionOccured = true;
		for (auto surf : hitboxSurfaces)
		{
			if (GetParam_T_SurfacePointProjection(*surf, point) < 0.0f)
			{
				collisionOccured = false;
				break;
			}
		}
	}
	else collisionOccured = false;

	return collisionOccured;
}

void CollisionDetection::createSurfaces(std::array<CollisionDetection::SurfacePtr, 6>& surfaces, const Hitbox& model)
{
	//			hitbox vert idx		hitbox normal
	//surface 1 [0, 1, 2, 3]		n[0]
	//surface 2 [4, 5, 6, 7]		n[1]
	//surface 3 [1, 5, 6, 2]		n[2]
	//surface 4 [2, 6, 7, 3]		n[3]
	//surface 5 [3, 7, 4, 0]		n[4]
	//surface 6 [1, 5, 4, 0]		n[5]

	SurfacePtr surf = std::make_shared<Surface>(model.transformNormals[0], model.transformVertices[0]);
	surfaces[0] = surf;
	surf = std::make_shared<Surface>(model.transformNormals[1], model.transformVertices[4]);
	surfaces[1] = surf;
	surf = std::make_shared<Surface>(model.transformNormals[2], model.transformVertices[1]);
	surfaces[2] = surf;
	surf = std::make_shared<Surface>(model.transformNormals[3], model.transformVertices[2]);
	surfaces[3] = surf;
	surf = std::make_shared<Surface>(model.transformNormals[4], model.transformVertices[3]);
	surfaces[4] = surf;
	surf = std::make_shared<Surface>(model.transformNormals[5], model.transformVertices[1]);
	surfaces[5] = surf;
}

void CollisionDetection::GetCollisionNormals(const Hitbox& model1, const Hitbox& model2, std::vector<glm::vec3>& collisionNormals) {
	std::map<int, Surface> surfaceCandidates;

	collisionNormals.clear();

	GetCollisonNormals_GetCandidates(model1, model2, surfaceCandidates);

	if (surfaceCandidates.size() == 0)
		return;
	else if (surfaceCandidates.size() == 1) {
		collisionNormals.push_back(surfaceCandidates.begin()->second.n);
		return;
	}
	else GetCollisionNormals_SortCandidates(model1, model2, surfaceCandidates, collisionNormals);
}

//g��wna funkcja algorytmu GJK
bool CollisionDetection::GJK(std::vector<glm::vec3>&model1, std::vector<glm::vec3>&model2) {
	unsigned GJK_it_limit = static_cast<unsigned>((model1.size()*model2.size()) / 2.0);												//g�rna granica ilo�ci iteracji dla algorytmu GJK
	unsigned int iterations = 0;																				//zmienna przechowuj�ca aktualn� warto�� iteracji algorytmu
																												//zabezpiecza przed ap�tleniem si� algorytmu
	std::vector<glm::vec3> Simplex;																				//vector opisuj�cy 'Sympleks' algorytmu GJK - punkty opisujace czworo�cian

	glm::vec3 d = GJK_StartVector(model1, model2);																	//obliczenie startowego wektora kierunkowego dla algorytmu GJK

	Simplex.push_back(GJK_SupportFunction(model1, model2, d));													//dodanie pierwszego punktu do Sympleksu przy u�yciu 'Support function' dla GJK

	d = -d;																								//zmienienie zwrotu wektora kierunkowego

	while (true) {
		if (iterations > GJK_it_limit)																			//je�li algorytm si� zap�tli�
			return true;																						//przerwij i zwr�� warto�� TRUE - kolizja zachodzi
																												//[je�li kolizja by nie zachodzi�a, zosta�oby to du�o wcze�niej wykryte]

		Simplex.push_back(GJK_SupportFunction(model1, model2, d));											//dodanie kolejnego punktu do Sympleksu

		if (glm::dot(Simplex[Simplex.size() - 1], d) < 0) {												//je�li dodany punkt nie przeszed� przez �rodek uk�adu
			return false;																					//kolizja nie zachodzi - zwr�� FALSE
		}
		else {																								//w przeciwnym wypadku
			if (GJK_SimplexContainsORIGIN(Simplex)) {														//sprawd� czy punkt (0,0,0) jest zawartty w Sympleksie
				return true;																				//je�li tak - kolizja zachodzi, zwr�� TRUE
			}
			else 																							//w przeciwnym wypadku
				d = GJK_GetDirection(Simplex);																//oblicz now� warto�� wektora kierunkowego
		}
		iterations++;																							//zwi�ksz licznik iteracji algorytmu
	}
}

//wyliczanie pocz�tkowego wektora kierunkowego
glm::vec3 CollisionDetection::GJK_StartVector(std::vector<glm::vec3>&model1, std::vector<glm::vec3>&model2) {
	//obliczenie punkt�w �rodkowych dla obu modeli przekazanych jako wektory punkt�w
	glm::vec3 midPoint1(0, 0, 0);
	glm::vec3 midPoint2(0, 0, 0);
	if (model1.size() >= model2.size()) {
		for (unsigned int i = 0; i < model1.size(); i++) {
			if (i < model2.size())
				midPoint2 += model2[i];
			midPoint1 += model1[i];
		}
	}
	else {
		for (unsigned int i = 0; i < model2.size(); i++) {
			if (i < model1.size())
				midPoint1 += model1[i];
			midPoint2 += model2[i];
		}
	}

	midPoint1 /= model1.size();
	midPoint2 /= model2.size();

	return (midPoint2 - midPoint1);																				//zwr�cenie wektora przesuni�cia miedzy �rodkami modelu 1 ,a modelu 2
}

//'Support function' dla GJK zwracaj�ca kolejny punkt do Sympleksu ze wzgl�du na wektor 'd'
glm::vec3 CollisionDetection::GJK_SupportFunction(std::vector<glm::vec3>&model1, std::vector<glm::vec3>&model2, glm::vec3 d) {
	//-------------------------------------------------------------------------------------------------OPIS----------------------------------------------------------------------------------------------
	//algorytm 'Support function' znajduje punkt w bryle 'model1' najbardziej oddalony w kierunku 'd' oraz punkt w bryle 'model2' najbardziej oddalony w kierunku '-d'
	//nast�pnie wyznacza punkt do Sympleksu poprzez r�nic� mi�dzy tymi dwoma punktami [wyznaczony punkt jest najbardziej oddalonym punktem w bryle r�nicy Minkowskiego dla modeli 1 i 2 w kierunku 'd']
	//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	glm::vec3 _d = -d;																						//stworzenie wektora o przeciwnym zwrocie do wektora kierunkowego d
	double dotProductResult1 = glm::dot(model1[0], d), dotProductResult2 = glm::dot(model2[0], _d);			//zainicjalizowanie warto�ci dla przeszukiwania zbioru punkt�w
	unsigned int idx1 = 0, idx2 = 0;

	for (unsigned int i = 1; i < model1.size(); i++) {
		if (glm::dot(model1[i], d) > dotProductResult1) {														//je�li iloczyn skalarny danego punktu z wek. 'd' jest wi�kszy od wcze�niejszego max
			idx1 = i;																							//to znaczy �e dany punkt jest bardziej oddalony w kierunku 'd' ni� wcze�niejsz
			dotProductResult1 = glm::dot(model1[i], d);														//zaktualizuj wart. dla najbardziej oddalonego punktu
		}
	}

	for (unsigned int i = 1; i < model2.size(); i++) {
		if (glm::dot(model2[i], _d) > dotProductResult2) {														//je�li iloczyn skalarny danego punktu z wek. '-d' jest wi�kszy od wcze�niejszego max
			idx2 = i;																							//to znaczy �e dany punkt jest bardziej oddalony w kierunku '-d' ni� wcze�niejszy				
			dotProductResult2 = glm::dot(model2[i], _d);														//zaktualizuj wart. dla najbardziej oddalonego punktu
		}
	}

	return (model1[idx1] - model2[idx2]);																		//wyznacz punkt b�d�cy r�nic� Minkowskiego dla znalezionych punkt�w dla modeli 1 i 2
}

//sprawdzanie czy Sympleks zawiera punkt (0,0,0)
bool CollisionDetection::GJK_SimplexContainsORIGIN(std::vector<glm::vec3>& Simplex) {
	if (Simplex.size() < 4)																						//je�li wielko�� Sympleksu jest mniejsz ni� 4
		return false;																							//Sympleks nie jest bry�a zwr�� FALSE
	else {
		glm::vec3 n[4];																							//normalne dla �cian Symplesku - skierowane na zewn�trz bry�y

		GJK_SimplexContainsORIGIN_CreateNormals(Simplex, n);													//za�adowanie normalnych 

		double Surface[4][4];																					//tablica opisuj�ca powierzchnie Sympleksu 

		GJK_SimplexContainsORIGIN_CreateSurfaces(Simplex, n, Surface);											//za�adowanie warto�ci tablicy 'Surface' - obliczenie wsp��zynnik�w powierzchni
																												//------------------------------------------------------------------------ALGORYTM-------------------------------------------------------------------------
																												//je�li parametr 't' dla prostej parametrycznej zaczynaj�cej si� w punkcie (0,0,0) i skierowanje w kierunku normalnej dla danej �ciany jest mniejszy od '0'
																												//punkt (0,0,0) nie znajduje si� wewn�trz Sympleksu - zwr�� FALSE
																												//je�li dla ka�dej normalnej paramtr 't' prostej jest wi�kszy od '0' - punkt (0,0,0)  znajduje si� wewn�trz Sympleksu - zwr�� TRUE
																												//---------------------------------------------------------------------------------------------------------------------------------------------------------
																												//zapis skr�cony powy�szego algorytmu po wyprowadzeniu konkretnych warunk�w dla znaku parametru 't'

		double t_mian[4], t_licz[4];
		for (unsigned i = 0; i < 4; i++) {																		//obliczenie wart. dla licznika i mianownika param. 't' dla ka�dej �ciany
			t_licz[i] = Surface[i][3];
			t_mian[i] = Surface[i][0] * n[i].x + Surface[i][1] * n[i].y + Surface[i][2] * n[i].z;
		}

		if ((t_licz[0] < 0 && t_mian[0] < 0) || (t_licz[0] > 0 && t_mian[0] > 0))								//sprawdzenie warunk�w czy 't < 0' dla ka�dej �ciany
			return false;
		if ((t_licz[1] < 0 && t_mian[1] < 0) || (t_licz[1] > 0 && t_mian[1] > 0))
			return false;
		if ((t_licz[2] < 0 && t_mian[2] < 0) || (t_licz[2] > 0 && t_mian[2] > 0))
			return false;
		if ((t_licz[3] < 0 && t_mian[3] < 0) || (t_licz[3] > 0 && t_mian[3] > 0))
			return false;

		return true;																							//je�li dla ka�dej �ciany parametr 't' ma warto�� dodatni�
																												//punkt (0,0,0) jest wewn�trz Sympleksu - zwr�� 'TRUE'
	}
}

//funkcja pomocnicza - budowanie normalnych dla Sympleksa
void CollisionDetection::GJK_SimplexContainsORIGIN_CreateNormals(std::vector<glm::vec3>& Simplex, glm::vec3 n[]) {
	//obliczenie normalnych do ka�dej �ciany Sympleksu
	glm::vec3 tmp1(Simplex[3] - Simplex[2]), tmp2(Simplex[1] - Simplex[2]);
	n[0] = glm::cross(tmp1, tmp2);
	tmp2 = Simplex[0] - Simplex[2];
	n[1] = glm::cross(tmp1, tmp2);
	tmp1 = Simplex[3] - Simplex[1]; tmp2 = Simplex[0] - Simplex[1];
	n[2] = glm::cross(tmp1, tmp2);
	tmp1 = Simplex[2] - Simplex[1];
	n[3] = glm::cross(tmp1, tmp2);

	//sprawdzenie czy ka�da normalna wskazuje na zewn�trz bry�y
	if (glm::dot(n[0], Simplex[0]) > glm::dot(n[0], Simplex[3]))
		n[0] = -n[0];
	if (glm::dot(n[1], Simplex[1]) > glm::dot(n[1], Simplex[3]))
		n[1] = -n[1];
	if (glm::dot(n[2], Simplex[2]) > glm::dot(n[2], Simplex[3]))
		n[2] = -n[2];
	if (glm::dot(n[3], Simplex[3]) > glm::dot(n[3], Simplex[2]))
		n[3] = -n[3];
}

//funkcja pomocnicza - budowanie powierzchni dla Sympleksa
void CollisionDetection::GJK_SimplexContainsORIGIN_CreateSurfaces(std::vector<glm::vec3>& Simplex, glm::vec3 n[], double Surface[][4]) {
	//przypisanie wsp�rz�dnych normalnych jako pierwszych 3 parametr�w dla ka�dej powierzchni (A, B, C)
	for (unsigned int i = 0; i < 3; i++) {
		Surface[0][i] = n[0][i];
		Surface[1][i] = n[1][i];
		Surface[2][i] = n[2][i];
		Surface[3][i] = n[3][i];
	}

	//obliczenie parametru D dla ka�dej powierzchni
	Surface[0][3] = -Surface[0][0] * Simplex[3].x - Surface[0][1] * Simplex[3].y - Surface[0][2] * Simplex[3].z;
	Surface[1][3] = -Surface[1][0] * Simplex[3].x - Surface[1][1] * Simplex[3].y - Surface[1][2] * Simplex[3].z;
	Surface[2][3] = -Surface[2][0] * Simplex[3].x - Surface[2][1] * Simplex[3].y - Surface[2][2] * Simplex[3].z;
	Surface[3][3] = -Surface[3][0] * Simplex[2].x - Surface[3][1] * Simplex[2].y - Surface[3][2] * Simplex[2].z;
}

//funkcja wyznaczaj�ca wektor kierunkowy w kolejnych iteracjach algorytmu
glm::vec3 CollisionDetection::GJK_GetDirection(std::vector<glm::vec3>&Simplex) {
	switch (Simplex.size()) {
	case 2:																										//dla Sympleksu o wielko�ci 2
		return GJK_GetDirection_Iteration_1(Simplex);															//pierwsza iteracja algorytmu GJK
		break;
	case 3:																										//dla Sympleksu o weilko�ci 3
		return GJK_GetDirection_Iteration_2(Simplex);															//druga iteracja algorytmu GJK
		break;
	case 4:																										//dla Sympleksu o wielko�ci 4
		return GJK_GetDirection_Iteration_3(Simplex);															//trzecia iteracja algorytmu GJK
		break;
	default:
		return glm::vec3(0.0, 0.0, 0.0);																			//dla ka�dej innej warto�ci Sympleksu zwr�� wektor zerowy - ochrona przed b��dami
		break;
	}
}

//Iteracja 1 [2 punkty w Sympleksie]
glm::vec3 CollisionDetection::GJK_GetDirection_Iteration_1(std::vector<glm::vec3>&Simplex) {
	//punkt A - ostatni punkt dodany do Sympleksu, B - pierwszy punkt dodany do Sympleksu,  d - wyznaczany wektor kierunkowy
	glm::vec3 d, ORIGIN(0, 0, 0), AB, A0;

	//obliczenie wektora 'd' na podstawie danych punkt�w w Sympleksie, oraz wiedzy �e punkt A znajduje si� po 'drugiej' stronie uk�adu wsp�rz�dnych ni� punkt B
	AB = Simplex[0] - Simplex[1];
	A0 = ORIGIN - Simplex[1];
	d = glm::cross(AB, A0);
	d = glm::cross(d, AB);
	return d;
}

//Iteracja 2 [3 punkty w Sympleksie]
glm::vec3 CollisionDetection::GJK_GetDirection_Iteration_2(std::vector<glm::vec3>&Simplex) {
	//punkt A - ostatni dodany punkt do Sympleksa, B - punkt dodany przed A, C - piewrszy punkt w Sympleksie
	glm::vec3 d, ORIGIN(0, 0, 0), AB, AC, A0;
	AB = Simplex[1] - Simplex[2]; AC = Simplex[0] - Simplex[2];													//inicjalizacja wektor�w
	A0 = ORIGIN - Simplex[2];

	//ustalenie nowej warto�ci wektora kierunkowego
	d = GJK_GetDirection_Iteration_2_AB(Simplex, AB, AC, A0);													//normlana do odcinka AB
	if (d != ORIGIN)
		return d;
	else {
		d = GJK_GetDirection_Iteration_2_AC(Simplex, AB, AC, A0);												//normalna do odcinka AC
		if (d != ORIGIN)
			return d;
		else {
			d = GJK_GetDirection_Iteration_2_CBA(AB, AC, A0);													//normalna do powierzchni CBA
			if (d != ORIGIN)
				return d;
			else {
				d = GJK_GetDirection_Iteration_2_ABC(AB, AC, A0);												//normalna do poweirzchni ABC
				return d;
			}
		}
	}
}

//sprawdzenie czy wektor normalny do odcinka AB jest skierowany do punktu (0,0,0)
glm::vec3 CollisionDetection::GJK_GetDirection_Iteration_2_AB(std::vector<glm::vec3>&Simplex, glm::vec3 AB, glm::vec3 AC, glm::vec3 A0) {
	glm::vec3 d;
	d = glm::cross(AC, AB);
	d = glm::cross(d, AB);																						//wyznaczenie wektora normalnego do odcinka AB

	if (glm::dot(d, A0) > 0) {																					//sprawdzenie czy wektor 'd' jest skierowany do �rodka uk�adu
		Simplex.erase(Simplex.begin());																			//jesli tak usu� pierwszy punkt z Sympleksu [C]
		return d;																								//zwr�� now� warto�� wektora kierunkowego
	}

	return glm::vec3(0, 0, 0);																						//w przeciwnym wypadku zwr�� wektor zerowy
}

//sprawdzenie czy wektor normalny do odcinka AC jest skierowany do punktu (0,0,0)
glm::vec3 CollisionDetection::GJK_GetDirection_Iteration_2_AC(std::vector<glm::vec3>&Simplex, glm::vec3 AB, glm::vec3 AC, glm::vec3 A0) {
	glm::vec3 d;
	d = glm::cross(AB, AC);
	d = glm::cross(d, AC);																						//wyznaczenie wektora normlanego do odcinka AC

	if (glm::dot(d, A0) > 0) {																					//sprawdzenie czy wektor 'd' jest skierowany do �rodka uk�adu
		Simplex.erase(Simplex.begin() + 1);																		//jesli tak usu� drugi element z Sympleksu [B]
		return d;																								//zwr�� now� warto�� wektora kierunkowego
	}

	return glm::vec3(0, 0, 0);																						//w przeciwnym wypadku zwr�� wektor zerowy
}

//sprawdzenie czy wektor normalny do powierzchni CBA jest skierowany do punktu (0,0,0)
glm::vec3 CollisionDetection::GJK_GetDirection_Iteration_2_CBA(glm::vec3 AB, glm::vec3 AC, glm::vec3 A0) {
	glm::vec3 d;
	d = glm::cross(AC, AB);																					//wyznaczenie wektora normalnego do poweirzchni CBA

	if (glm::dot(d, A0) > 0)																					//je�li wektor 'd' jest skierowny do �rodka uk�adu
		return d;																								//zachowaj wszystkie elementy Sympleksu i zwr�� now� warto�� wektora kierunkowego

	return glm::vec3(0, 0, 0);																						//w przeciwnym wypadku zwr�� wektor zerowy
}

//sprawdzenie czy wektor normalny do powierzchni ABC jest skierowany do punktu (0,0,0)
glm::vec3 CollisionDetection::GJK_GetDirection_Iteration_2_ABC(glm::vec3 AB, glm::vec3 AC, glm::vec3 A0) {
	glm::vec3 d;
	d = glm::cross(AB, AC);																					//wyznaczenie wektora normalnego do powierzchni ABC

	return d;																									//zwr�� wektor 'd' jako now� warto�� wektora kierunkowego
																												//ostatni warunek w 2 iteracji algorytmu - nie przeprowadza si� �adnych dodatkowych test�w
}

//Iteracja 3 [4 punkty w Sympleksie]
glm::vec3 CollisionDetection::GJK_GetDirection_Iteration_3(std::vector<glm::vec3>&Simplex) {
	glm::vec3 d, ORIGIN(0, 0, 0), AB, AC, AD, A0;
	AB = Simplex[2] - Simplex[3]; AC = Simplex[1] - Simplex[3];
	AD = Simplex[0] - Simplex[3]; A0 = ORIGIN - Simplex[3];

	//ustalenie nowej warto�ci wektora kierunkowego
	d = GJK_GetDirection_Iteration_3_AB(Simplex, AB, AC, A0);													//normalna do odcinka AB									
	if (d != ORIGIN)
		return d;
	else {
		d = GJK_GetDirection_Iteration_3_AC(Simplex, AB, AC, A0);												//normalna do odcinka AC
		if (d != ORIGIN)
			return d;
		else {
			d = GJK_GetDirection_Iteration_3_AD(Simplex, AC, AD, A0);											//normalna do odcinka AD
			if (d != ORIGIN)
				return d;
			else {
				d = GJK_GetDirection_Iteration_3_ABC(Simplex, AB, AC, A0);										//normalna do p�aszczyzny ABC
				if (d != ORIGIN)
					return d;
				else {
					d = GJK_GetDirection_Iteration_3_ABD(Simplex, AB, AD, A0);									//normalna do p�aszczyzny ABD
					if (d != ORIGIN)
						return d;
					else {
						d = GJK_GetDirection_Iteration_3_ACD(Simplex, AD, AC, A0);									//normalna do p�aszczyzny ACD
						return d;
					}
				}
			}
		}
	}
}

//sprawdzenie czy wektor normalny do odcinka AB jest skierowany do punktu (0,0,0)
glm::vec3 CollisionDetection::GJK_GetDirection_Iteration_3_AB(std::vector<glm::vec3>&Simplex, glm::vec3 AB, glm::vec3 AC, glm::vec3 A0) {
	glm::vec3 d;
	d = glm::cross(AC, AB);
	d = glm::cross(d, AB);																						//wyznaczenie wektora normlanego do odcinka AB

	if (glm::dot(d, A0) > 0) {																					//je�li wektor 'd' jest skierowany do punktu (0,0,0)
		Simplex.erase(Simplex.begin());																			//usu� dwa pierwsze elementy z Sympleksu (C i D)
		Simplex.erase(Simplex.begin());
		return d;																								//zwr�� now� warto�� wektora kierunkowego 
	}

	return glm::vec3(0, 0, 0);																						//w przeciwnym wypadku zwr�� wektor zerowy
}

//sprawdzenie czy wektor normalny do odcinka AC jest skierowany do punktu (0,0,0)
glm::vec3 CollisionDetection::GJK_GetDirection_Iteration_3_AC(std::vector<glm::vec3>&Simplex, glm::vec3 AB, glm::vec3 AC, glm::vec3 A0) {
	glm::vec3 d;
	d = glm::cross(AB, AC);
	d = glm::cross(d, AC);																						//wyznaczenie wektora normalnego do odcinka AC

	if (glm::dot(d, A0) > 0) {																					//je�li wektor 'd' jest skierowany do punktu (0,0,0)
		Simplex.erase(Simplex.begin());																			//usu� pierwszy i trzeci punkt z Sympleksu (D i B)
		Simplex.erase(Simplex.begin() + 1);
		return d;																								//zwr�� now� warto�� wektora kierunkowego
	}

	return glm::vec3(0, 0, 0);																						//w przeciwnym wypadku zwr�� wektor zerowy
}

//sprawdzenie czy wektor normalny do odcinka AD jest skierowany do punktu (0,0,0)
glm::vec3 CollisionDetection::GJK_GetDirection_Iteration_3_AD(std::vector<glm::vec3>&Simplex, glm::vec3 AC, glm::vec3 AD, glm::vec3 A0) {
	glm::vec3 d;
	d = glm::cross(AC, AD);
	d = glm::cross(d, AD);																						//wyznaczenie wektora normalnego do odcinka AD

	if (glm::dot(d, A0) > 0) {																					//je�li wektor 'd' jest skierowany do punktu (0,0,0)
		Simplex.erase(Simplex.begin() + 1);																		//usu� drugi i trzeci punkt z Sympleksu (B i C)
		Simplex.erase(Simplex.begin() + 1);
		return d;																								//zwr�� now� warto�� wektora kierunkowego
	}

	return glm::vec3(0, 0, 0);																						//w przeciwnym wypadku zwr�� wektor zerowy
}

//sprawdzenie czy wektor normalny do poweirzchni ABC jest skierowany do punktu (0,0,0)
glm::vec3 CollisionDetection::GJK_GetDirection_Iteration_3_ABC(std::vector<glm::vec3>&Simplex, glm::vec3 AB, glm::vec3 AC, glm::vec3 A0) {
	glm::vec3 d;
	d = glm::cross(AC, AB);																					//wyznaczenie wektora normalnego do powierzchni  ABC 

	if (glm::dot(d, Simplex[0]) > glm::dot(d, Simplex[3]))													//sprawdzenie czy normalna jest skierowana na zewn�trz Sympleksu
		d = -d;

	if (glm::dot(d, A0) > 0) {																					//je�li wektor 'd' jest skierowany do punktu (0,0,0)
		Simplex.erase(Simplex.begin());																			//usu� pierwszy punkt z Sympleksu (D)
		return d;																								//zwr�� now� warto�� wektora kierunkowego
	}

	return glm::vec3(0, 0, 0);																						//w przeciwnym wypadku zwr�� wektor zerowy
}

//sprawdzenie czy wektor normalny do powierzchni ABD jest skierowany do (0,0,0)
glm::vec3 CollisionDetection::GJK_GetDirection_Iteration_3_ABD(std::vector<glm::vec3>&Simplex, glm::vec3 AB, glm::vec3 AD, glm::vec3 A0) {
	glm::vec3 d;
	d = glm::cross(AD, AB);																					//wyznaczenie wektora normalnego do poweirzchni ABD

	if (glm::dot(d, Simplex[1]) > glm::dot(d, Simplex[3]))													//sprawdzenie czy normalna jest skierowana na zewn�trz Sympleksu
		d = -d;

	if (glm::dot(d, A0) > 0) {																					//je�li wektor 'd' jest skierowany do punktu (0,0,0) 
		Simplex.erase(Simplex.begin() + 1);																		//usu� drugi element z Sympleksu (C)
		return d;																								//zwr�� now� warto�� wektora kierunkowego
	}

	return glm::vec3(0, 0, 0);																						//w przeciwnym wypadku zwr�� wektor zerowy
}

//sprawdzenie czy wektor normalny do powierzchni ACD jest skierowany do (0,0,0)
glm::vec3 CollisionDetection::GJK_GetDirection_Iteration_3_ACD(std::vector<glm::vec3>&Simplex, glm::vec3 AD, glm::vec3 AC, glm::vec3 A0) {
	glm::vec3 d;
	d = glm::cross(AD, AC);																					//wyznaczenie wektora normalnego do poweirzchni ACD

	if (glm::dot(d, Simplex[2]) > glm::dot(d, Simplex[3]))													//sprawdzenie czy normalna jest skierowana na zewn�trz Sympleksu
		d = -d;

	Simplex.erase(Simplex.begin() + 2);																			//usu� 3 element z Sympleksu
	return d;																									//zwr�� now� warto�� wektora kierunkowego
																												//ostatni warunek 3 iteracji algorytmu - nie jest sprawdzany �aden dodatkowy warunek
}

glm::vec3 CollisionDetection::GetSurfaceCrossPoint(const Surface& surface, const Line& line) {
	float t, div;
	glm::vec3 outPt;

	t = -(surface.A*line.pt.x + surface.B*line.pt.y + surface.C*line.pt.z + surface.D);

	div = (surface.A*line.vec.x + surface.B*line.vec.y + surface.C*line.vec.z);

	if (div != 0.0f) t /= div;
	else throw std::exception();

	outPt = line.pt + t * line.vec;

	return outPt;
}
float CollisionDetection::GetParam_T_SurfaceCrossPoint(const Surface& surface, const Line& line) {
	float t, div;

	t = -(surface.A*line.pt.x + surface.B*line.pt.y + surface.C*line.pt.z + surface.D);

	div = (surface.A*line.vec.x + surface.B*line.vec.y + surface.C*line.vec.z);

	if (div != 0.0f) t /= div;
	else throw std::exception();

	return t;
}

glm::vec3 CollisionDetection::GetSurfacePointProjection(const Surface& surface, glm::vec3 pt) {
	float t, div;
	glm::vec3 outPt;

	t = -(surface.A*pt.x + surface.B*pt.y + surface.C*pt.z + surface.D);

	div = (surface.A*surface.n.x + surface.B*surface.n.y + surface.C*surface.n.z);

	if (div != 0.0f) t /= div;
	else throw std::exception();

	outPt = pt + t * surface.n;

	return outPt;
}

float CollisionDetection::GetParam_T_SurfacePointProjection(const Surface& surface, glm::vec3 pt) {
	float t, div;

	t = -(surface.A*pt.x + surface.B*pt.y + surface.C*pt.z + surface.D);

	div = (surface.A*surface.n.x + surface.B*surface.n.y + surface.C*surface.n.z);

	if (div != 0.0f) t /= div;
	//else throw std::exception();

	return t;
}

bool CollisionDetection::IfPointInRectangle(glm::vec3 rect1, glm::vec3 rect2, glm::vec3 rect3, glm::vec3 rect4, glm::vec3 checkPt) {
	//rect points in order [rect1->rect2->rect3->rect4->rect1]
	Surface s1(glm::vec3(rect1 - rect4), rect1);
	Surface s2(glm::vec3(rect2 - rect1), rect2);
	Surface s3(glm::vec3(rect3 - rect2), rect3);
	Surface s4(glm::vec3(rect4 - rect3), rect4);

	if (GetParam_T_SurfacePointProjection(s1, checkPt) < 0) return false;
	if (GetParam_T_SurfacePointProjection(s2, checkPt) < 0) return false;
	if (GetParam_T_SurfacePointProjection(s3, checkPt) < 0) return false;
	if (GetParam_T_SurfacePointProjection(s4, checkPt) < 0) return false;

	return true;
}

bool CollisionDetection::IfPointInTriangle(glm::vec3 tr1, glm::vec3 tr2, glm::vec3 tr3, glm::vec3 checkPt) {
	Surface s[3];
	glm::vec3 n;
	glm::vec3 trNorm;

	trNorm = glm::cross((tr2 - tr1), (tr3 - tr1));

	n = glm::cross((tr2 - tr1), trNorm);
	s[0] = Surface(n, tr1);
	n = glm::cross((tr3 - tr2), trNorm);
	s[1] = Surface(n, tr2);
	n = glm::cross((tr1 - tr3), trNorm);
	s[2] = Surface(n, tr3);

	for (int i = 0; i < 3; ++i) {
		if (GetParam_T_SurfacePointProjection(s[i], checkPt) < 0) return false;
	}

	return true;
}

bool CollisionDetection::IfPointInHitbox(const Hitbox& hitbox, glm::vec3 pt) {
	Surface s[6];

	s[0] = Surface(hitbox.transformNormals[0], hitbox.transformVertices[0]);
	s[1] = Surface(hitbox.transformNormals[1], hitbox.transformVertices[0]);
	s[2] = Surface(hitbox.transformNormals[2], hitbox.transformVertices[1]);
	s[3] = Surface(hitbox.transformNormals[3], hitbox.transformVertices[2]);
	s[4] = Surface(hitbox.transformNormals[4], hitbox.transformVertices[3]);
	s[5] = Surface(hitbox.transformNormals[5], hitbox.transformVertices[4]);

	for (int i = 0; i < 6; ++i)
		if (GetParam_T_SurfacePointProjection(s[i], pt) < 0) return false;

	return true;
}

bool CollisionDetection::IfPointInHitbox(const Surface surfaces[], glm::vec3 pt) {
	for (int i = 0; i < 6; ++i)
		if (GetParam_T_SurfacePointProjection(surfaces[i], pt) < 0) return false;

	return true;
}

void CollisionDetection::GetCollisonNormals_GetCandidates(const Hitbox& model1, const Hitbox& model2, std::map<int, Surface>& surfaceCandidates) {
	glm::vec3 midPt(0.0f);
	Surface model2_s[6];

	surfaceCandidates.clear();

	for (int i = 0; i < 8; ++i)
		midPt += model1.transformVertices[i];

	midPt /= 8.0f;

	model2_s[0] = Surface(model2.transformNormals[0], model2.transformVertices[0]);
	model2_s[1] = Surface(model2.transformNormals[1], model2.transformVertices[0]);
	model2_s[2] = Surface(model2.transformNormals[2], model2.transformVertices[1]);
	model2_s[3] = Surface(model2.transformNormals[3], model2.transformVertices[2]);
	model2_s[4] = Surface(model2.transformNormals[4], model2.transformVertices[3]);
	model2_s[5] = Surface(model2.transformNormals[5], model2.transformVertices[4]);

	for (int i = 0; i < 6; ++i) {
		if (GetParam_T_SurfacePointProjection(model2_s[i], midPt) < 0)
			surfaceCandidates.insert(std::map<int, Surface>::value_type(i, model2_s[i]));
	}
}

void CollisionDetection::GetCollisionNormals_SortCandidates(const Hitbox& model1, const Hitbox& model2, std::map<int, Surface>& surfaceCandidates, std::vector<glm::vec3>& collisionNormals) {
	glm::vec3 surfaceMidPt;
	glm::vec3 surfVec1, surfVec2;
	glm::vec3 model1_midPt(0.0f);
	glm::vec3 surfaceModel1MidPtProj;

	const int testPointsCount = 10;
	float model1_midPtSurfaceDist;

	std::vector<glm::vec3> testPoints;

	collisionNormals.clear();

	for (int i = 0; i < 8; ++i)
		model1_midPt += model1.transformVertices[i];
	model1_midPt /= 8.0f;

	Surface model1_surafaces[6];
	model1_surafaces[0] = Surface(model1.transformNormals[0], model1.transformVertices[0]);
	model1_surafaces[1] = Surface(model1.transformNormals[1], model1.transformVertices[0]);
	model1_surafaces[2] = Surface(model1.transformNormals[2], model1.transformVertices[1]);
	model1_surafaces[3] = Surface(model1.transformNormals[3], model1.transformVertices[2]);
	model1_surafaces[4] = Surface(model1.transformNormals[4], model1.transformVertices[3]);
	model1_surafaces[5] = Surface(model1.transformNormals[5], model1.transformVertices[4]);

	for (std::map<int, Surface>::iterator it = surfaceCandidates.begin(); it != surfaceCandidates.end(); ++it) {
		GetCollisionNormals_SortCandidates_GetSurfaceParams(model2, it->first, &surfaceMidPt, &surfVec1, &surfVec2);
		surfaceModel1MidPtProj = GetSurfacePointProjection(it->second, model1_midPt);
		model1_midPtSurfaceDist = glm::length(surfaceModel1MidPtProj - model1_midPt);
		GetCollisionNormals_SortCandidates_GenerateTestPoints(surfaceMidPt, surfVec1, surfVec2, surfaceModel1MidPtProj, testPointsCount, testPoints);

		for (int i = 0; i < testPoints.size(); ++i) {
			//if (IfPointInHitbox(model1, testPoints[i] + (model1_midPtSurfaceDist / 2.0f)*it->second.n) == true) {
			if (IfPointInHitbox(model1_surafaces, testPoints[i] + (model1_midPtSurfaceDist / 2.0f)*it->second.n) == true) {
				collisionNormals.push_back(it->second.n);
				break;
			}
		}
	}
}

void CollisionDetection::GetCollisionNormals_SortCandidates_GetSurfaceParams(const Hitbox& model2, int idx, glm::vec3* outSurfaceMidPt, glm::vec3* outSurfaceVec1, glm::vec3* outSurfaceVec2) {
	*outSurfaceMidPt = glm::vec3(0.0f);
	switch (idx) {
	case 0: {
		*outSurfaceMidPt += model2.transformVertices[0];
		*outSurfaceMidPt += model2.transformVertices[1];
		*outSurfaceMidPt += model2.transformVertices[2];
		*outSurfaceMidPt += model2.transformVertices[3];
		*outSurfaceMidPt /= 4.0f;
		*outSurfaceVec1 = model2.transformVertices[1] - model2.transformVertices[0];
		*outSurfaceVec2 = model2.transformVertices[1] - model2.transformVertices[2];
		break;
	}
	case 1: {
		*outSurfaceMidPt += model2.transformVertices[0];
		*outSurfaceMidPt += model2.transformVertices[1];
		*outSurfaceMidPt += model2.transformVertices[5];
		*outSurfaceMidPt += model2.transformVertices[4];
		*outSurfaceMidPt /= 4.0f;
		*outSurfaceVec1 = model2.transformVertices[0] - model2.transformVertices[1];
		*outSurfaceVec2 = model2.transformVertices[0] - model2.transformVertices[4];
		break;
	}
	case 2: {
		*outSurfaceMidPt += model2.transformVertices[1];
		*outSurfaceMidPt += model2.transformVertices[2];
		*outSurfaceMidPt += model2.transformVertices[6];
		*outSurfaceMidPt += model2.transformVertices[5];
		*outSurfaceMidPt /= 4.0f;
		*outSurfaceVec1 = model2.transformVertices[1] - model2.transformVertices[2];
		*outSurfaceVec2 = model2.transformVertices[1] - model2.transformVertices[5];
		break;
	}
	case 3: {
		*outSurfaceMidPt += model2.transformVertices[2];
		*outSurfaceMidPt += model2.transformVertices[3];
		*outSurfaceMidPt += model2.transformVertices[7];
		*outSurfaceMidPt += model2.transformVertices[6];
		*outSurfaceMidPt /= 4.0f;
		*outSurfaceVec1 = model2.transformVertices[2] - model2.transformVertices[3];
		*outSurfaceVec2 = model2.transformVertices[2] - model2.transformVertices[6];
		break;
	}
	case 4: {
		*outSurfaceMidPt += model2.transformVertices[3];
		*outSurfaceMidPt += model2.transformVertices[0];
		*outSurfaceMidPt += model2.transformVertices[4];
		*outSurfaceMidPt += model2.transformVertices[7];
		*outSurfaceMidPt /= 4.0f;
		*outSurfaceVec1 = model2.transformVertices[3] - model2.transformVertices[0];
		*outSurfaceVec2 = model2.transformVertices[3] - model2.transformVertices[7];
		break;
	}
	case 5: {
		*outSurfaceMidPt += model2.transformVertices[4];
		*outSurfaceMidPt += model2.transformVertices[5];
		*outSurfaceMidPt += model2.transformVertices[6];
		*outSurfaceMidPt += model2.transformVertices[7];
		*outSurfaceMidPt /= 4.0f;
		*outSurfaceVec1 = model2.transformVertices[4] - model2.transformVertices[5];
		*outSurfaceVec2 = model2.transformVertices[4] - model2.transformVertices[7];
		break;
	}
	}
}

void CollisionDetection::GetCollisionNormals_SortCandidates_GenerateTestPoints(const glm::vec3& surfMidPt, const glm::vec3& surfVec1, const glm::vec3& surfVec2, const glm::vec3& surfPtProj,
	const int testPtCount, std::vector<glm::vec3>& testPoints) {
	float dotVec1, dotVec2;
	glm::vec3 vecMidSurfPt(surfPtProj - surfMidPt);

	testPoints.clear();

	dotVec1 = glm::dot(vecMidSurfPt, surfVec1);
	dotVec2 = glm::dot(vecMidSurfPt, surfVec2);

	//boost::random::mt19937 rng;
	//boost::random::uniform_int_distribution<> randFun(0, 100);
	std::mt19937 rng;
	std::uniform_int_distribution<> randFun(0, 100);
	
	int randomVal1, randomVal2;
	glm::vec3 outPt;
	for (int i = 0; i < testPtCount; ++i) {
		randomVal1 = randFun(rng);
		randomVal2 = randFun(rng);

		if (dotVec1 >= 0 && dotVec2 >= 0) {
			outPt = surfMidPt + (static_cast<float>(randomVal1) / 100.0f)*surfVec1 + (static_cast<float>(randomVal2) / 100.0f)*surfVec2;
			testPoints.push_back(outPt);
		}
		else if (dotVec1 >= 0 && dotVec2 < 0) {
			outPt = surfMidPt + (static_cast<float>(randomVal1) / 100.0f)*surfVec1 - (static_cast<float>(randomVal2) / 100.0f)*surfVec2;
			testPoints.push_back(outPt);
		}
		else if (dotVec1 < 0 && dotVec2 < 0) {
			outPt = surfMidPt - (static_cast<float>(randomVal1) / 100.0f)*surfVec1 - (static_cast<float>(randomVal2) / 100.0f)*surfVec2;
			testPoints.push_back(outPt);
		}
		else if (dotVec1 < 0 && dotVec2 >= 0) {
			outPt = surfMidPt - (static_cast<float>(randomVal1) / 100.0f)*surfVec1 + (static_cast<float>(randomVal2) / 100.0f)*surfVec2;
			testPoints.push_back(outPt);
		}
	}
}
