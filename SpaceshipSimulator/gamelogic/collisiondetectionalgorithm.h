#pragma once
#include <vector>
#include <random>
#include <glm/gtc/matrix_transform.hpp>
#include <GameResources/gameobjects/hitbox.h>

const unsigned int RayCastLinesNum = 128;																					//liczba prostych u�ywanych przy algorytmie 'Ray Cast'

struct ModelCollision {
	bool status;																											//status kolizji: TRUE - kolizja zasz�a, FALSE - nie by�o kolizji
																															//sk�adowe klasy okre�laj�ce detekcj� kolizji tylko z g��wnym hitboxem modelu/obiektu
																															//u�ywane je�li dany model/obiekt posiada TYLKO g��wnego hitboxa
																															//je�li zaszla kolizja z g��wnym hitboxem modelu ['Character']:						'mainColBox = TRUE', 'mainColBoxObjIdx = -1'
																															//je�li zasz�a kolizja z g��wnym hitboxem obiektu ['Object' dla klasy 'Character']: 'mainColBox = TRUE', 'mainColBoxObjIdx = indeks_obiektu'
	bool mainColBox;																										//status kolizji z g��wnym hitboxem modelu/obiektu
	int mainColBoxObjIdx;																									//indeks obiektu dla kt�rego zasz�a kolizja z g��wnym hitboxem
																															//wektor przechowuj�cy pary (objIdx, colBoxIdx)
																															//'objIdx' - indeks obiektu z kt�ym zasz�a kolizja dla aktualnie sprawdzanego modelu
																															//'colBoxIdx' - indeks dla bry�y kolizji dla obiektu opisanego poprzez 'indeks obiektu' z kt�r� zasz�a kolizja dla aktualnie sprawdzanego modelu
	std::vector<std::pair<unsigned int, unsigned int>> colIdx;

	ModelCollision() :status(false), mainColBox(false), mainColBoxObjIdx(-1) {}
};

//wektor przechowuj�cy pary (normal, linesCrossed) dla danej pary (objIdx,colBoxIdx)
//'normal' - wektor normalny do �ciany badanej bry�y kolizji ('colBoxIdx')
//'linesCrossed' - liczba prostych przechodz�cych przez �cian� opisan� przez normaln� 'normal' podczas testu 'Ray Cast' dla aktualnie badanej bry�y kolizji
typedef std::vector<std::pair<glm::vec3, unsigned int>> CollisionNormals;

class CollisionDetection {
public:
	struct Surface {
		float A, B, C, D;
		glm::vec3 n;

		Surface() :A(0.0f), B(0.0f), C(0.0f), D(0.0f), n(glm::vec3(0.0f)) {}
		Surface(const glm::vec3& n, const glm::vec3& pt) {
			this->n = n;
			A = n.x;
			B = n.y;
			C = n.z;
			D = -(A*pt.x + B * pt.y + C * pt.z);
		}
		Surface(const glm::vec3& pt1, const glm::vec3& pt2, const glm::vec3& pt3) {
			n = glm::cross((pt2 - pt1), (pt3 - pt1));
			n = glm::normalize(n);
			A = n.x;
			B = n.y;
			C = n.z;
			D = -(A*pt1.x + B * pt1.y + C * pt1.z);
		}

		float operator[](int idx) {
			switch (idx) {
			case 0:
				return A;
			case 1:
				return B;
			case 2:
				return C;
			case 3:
				return D;
			default:
				throw std::exception();
			}
		}
	};

	struct Line {
		glm::vec3 vec;
		glm::vec3 pt;

		Line() :vec(glm::vec3(0.0f)), pt(glm::vec3(0.0f)) {}
		Line(const glm::vec3& vec_, const glm::vec3& pt_) : vec(vec_), pt(pt_) {}
	};
private:
	//-----------------------------------------------------------------------------------------------------------------------------
	//--------------------------------------------------------ALGORYTM GJK---------------------------------------------------------
	//------------------------------------------------funckje tworz�ce algorytm GJK------------------------------------------------
	//-----------------------------------------------------------------------------------------------------------------------------
	static bool GJK(std::vector<glm::vec3>&model1, std::vector<glm::vec3>&model2);												//g��wna funkcja algorytmu GJK

	static glm::vec3 GJK_StartVector(std::vector<glm::vec3>&model1, std::vector<glm::vec3>&model2);									//wyliczanie pocz�tkowego wektora kierunkowego

	static glm::vec3 GJK_SupportFunction(std::vector<glm::vec3>&model1, std::vector<glm::vec3>&model2, glm::vec3 d);						//'Support function' dla GJK

	static bool GJK_SimplexContainsORIGIN(std::vector<glm::vec3>& Simplex);													//sprawdzanie czy Sympleks zawiera punkt (0,0,0)
	static void GJK_SimplexContainsORIGIN_CreateNormals(std::vector<glm::vec3>& Simplex, glm::vec3 n[]);							//funkcja pomocnicza - budowanie normalnych dla Sympleksa
	static void GJK_SimplexContainsORIGIN_CreateSurfaces(std::vector<glm::vec3>& Simplex, glm::vec3 n[], double Surface[][4]);	//funkcja pomocnicza - budowanie powierzchni dla Sympleksa

	static glm::vec3 GJK_GetDirection(std::vector<glm::vec3>&Simplex);															//funkcja wyznaczaj�ca wektor kierunkowy w kolejnych iteracjach algorytmu

																																//Iteracja 1 [2 punkty w Sympleksie]
	static glm::vec3 GJK_GetDirection_Iteration_1(std::vector<glm::vec3>&Simplex);
	//Iteracja 2 [3 punkty w Sympleksie]
	static glm::vec3 GJK_GetDirection_Iteration_2(std::vector<glm::vec3>&Simplex);
	static glm::vec3 GJK_GetDirection_Iteration_2_AB(std::vector<glm::vec3>&Simplex, glm::vec3 AB, glm::vec3 AC, glm::vec3 A0);
	static glm::vec3 GJK_GetDirection_Iteration_2_AC(std::vector<glm::vec3>&Simplex, glm::vec3 AB, glm::vec3 AC, glm::vec3 A0);
	static glm::vec3 GJK_GetDirection_Iteration_2_CBA(glm::vec3 AB, glm::vec3 AC, glm::vec3 A0);
	static glm::vec3 GJK_GetDirection_Iteration_2_ABC(glm::vec3 AB, glm::vec3 AC, glm::vec3 A0);
	//Iteracja 3 [4 punkty w Sympleksie]
	static glm::vec3 GJK_GetDirection_Iteration_3(std::vector<glm::vec3>&Simplex);
	static glm::vec3 GJK_GetDirection_Iteration_3_AB(std::vector<glm::vec3>&Simplex, glm::vec3 AB, glm::vec3 AC, glm::vec3 A0);
	static glm::vec3 GJK_GetDirection_Iteration_3_AC(std::vector<glm::vec3>&Simplex, glm::vec3 AB, glm::vec3 AC, glm::vec3 A0);
	static glm::vec3 GJK_GetDirection_Iteration_3_AD(std::vector<glm::vec3>&Simplex, glm::vec3 AC, glm::vec3 AD, glm::vec3 A0);
	static glm::vec3 GJK_GetDirection_Iteration_3_ABC(std::vector<glm::vec3>&Simplex, glm::vec3 AC, glm::vec3 AB, glm::vec3 A0);
	static glm::vec3 GJK_GetDirection_Iteration_3_ABD(std::vector<glm::vec3>&Simplex, glm::vec3 AB, glm::vec3 AD, glm::vec3 A0);
	static glm::vec3 GJK_GetDirection_Iteration_3_ACD(std::vector<glm::vec3>&Simplex, glm::vec3 AD, glm::vec3 AC, glm::vec3 A0);

	static void GetCollisonNormals_GetCandidates(const Hitbox& model1, const Hitbox& model2, std::map<int, Surface>& surfaceCandidates);
	static void GetCollisionNormals_SortCandidates(const Hitbox& model1, const Hitbox& model2, std::map<int, Surface>& surfaceCandidates, std::vector<glm::vec3>& collisionNormals);
	static void GetCollisionNormals_SortCandidates_GetSurfaceParams(const Hitbox& model2, int idx, glm::vec3* outSurfaceMidPt, glm::vec3* outSurfaceVec1, glm::vec3* outSurfaceVec2);
	static void GetCollisionNormals_SortCandidates_GenerateTestPoints(const glm::vec3& surfMidPt, const glm::vec3& surfVec1, const glm::vec3& surfVec2, const glm::vec3& surfPtProj,
		const int testPtCount, std::vector<glm::vec3>& testPoints);

public:
	static bool CheckCollision(const Hitbox& model1, const Hitbox& model2);												//sprawdzanie kolizji pomi�dzy dwoma modelami

	static void GetCollisionNormals(const Hitbox& model1, const Hitbox& model2, std::vector<glm::vec3>& collisionNormals);

	static glm::vec3 GetSurfaceCrossPoint(const Surface& surface, const Line& line);
	static float GetParam_T_SurfaceCrossPoint(const Surface& surface, const Line& line);
	static glm::vec3 GetSurfacePointProjection(const Surface& surface, glm::vec3 pt);
	static float GetParam_T_SurfacePointProjection(const Surface& surface, glm::vec3 pt);

	static bool IfPointInRectangle(glm::vec3 rect1, glm::vec3 rect2, glm::vec3 rect3, glm::vec3 rect4, glm::vec3 checkPt);
	static bool IfPointInTriangle(glm::vec3 tr1, glm::vec3 tr2, glm::vec3 tr3, glm::vec3 checkPt);
	static bool IfPointInHitbox(const Hitbox& hitbox, glm::vec3 pt);
	static bool IfPointInHitbox(const Surface surfaces[], glm::vec3 pt);
};