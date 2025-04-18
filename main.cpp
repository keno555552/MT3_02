#include <Novice.h>
#include "myMath.h"
#include "ImGui.h"

const char kWindowTitle[] = "GC01_05_カン_ケンリャン";

int kWindowWidth = 1280, kWindowHeight = 720;

///=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
/// 重要定義、構造体作り
///=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

/// 計算に使う値
Segment segment{ {-2.0f, -1.0f, 0.0f}, {3.0f, 2.0f, 2.0f} };
Vector3 point{ -1.5f, 0.6f, 0.6f };

Vector3 project = Project(Subtract(point, segment.origin), segment.diff);
Vector3 closestPoint = ClosestPoint(point, segment);

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, kWindowWidth, kWindowHeight);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	///=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
	/// 初期化
	///=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

	Sphere pointSphere{ point,0.01f };
	Sphere closestPointSphere { closestPoint , 0.01f };
	
	///カメラ初期化
	Vector3 cameraTranslate{ 0.0f, 1.9f, -6.49f };
	Vector3 cameraRotate{ 0.26f, 0.0f, 0.0f };

	Matrix4x4 cameraMatix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, cameraRotate, cameraTranslate);
	Matrix4x4 cameraViewMatrix = Inverse(cameraMatix);
	Matrix4x4 cameraProjectionMatrix = MakePerspectiveFovMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);
	Matrix4x4 cameraWorldViewProjectionMatrix = cameraProjectionMatrix * cameraViewMatrix;
	Matrix4x4 cameraViewportMatrix = MakeViewportMatrix(0.0f, 0.0f, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
		/// ボタン処理
		///=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

		if (keys[DIK_A]) { cameraTranslate.x += 0.1f; }
		if (keys[DIK_D]) { cameraTranslate.x -= 0.1f; }
		if (keys[DIK_W]) { cameraTranslate.z += 0.1f; }
		if (keys[DIK_S]) { cameraTranslate.z -= 0.1f; }
		if (keys[DIK_Z]) { cameraRotate.y += 0.1f; }
		if (keys[DIK_C]) { cameraRotate.y -= 0.1f; }
		if (keys[DIK_R]) {
			cameraTranslate = { 0.0f, 1.9f, -6.49f };
			cameraRotate = { 0.26f, 0.0f,   0.0f };
		}

		///=========================================================================================================================================================================================
		/// 更新処理
		///=========================================================================================================================================================================================


		//数値改変の更新処理
		project = Project(Subtract(point, segment.origin), segment.diff);
		closestPoint = ClosestPoint(point, segment);
		pointSphere = { point,0.01f };
		closestPointSphere = { closestPoint , 0.01f };

		///カメラ更新処理
		cameraMatix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, cameraRotate, cameraTranslate);
		cameraViewMatrix = Inverse(cameraMatix);
		cameraProjectionMatrix = MakePerspectiveFovMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);
		cameraWorldViewProjectionMatrix = cameraViewMatrix * cameraProjectionMatrix;
		cameraViewportMatrix = MakeViewportMatrix(0.0f, 0.0f, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);


		///=========================================================================================================================================================================================
		/// 描画処理
		///=========================================================================================================================================================================================

		/// Grid
		DrawGrid(cameraWorldViewProjectionMatrix, cameraViewportMatrix);

		///点
		DrawSphere(pointSphere, cameraWorldViewProjectionMatrix, cameraViewportMatrix, RED);
		DrawSphere(closestPointSphere, cameraWorldViewProjectionMatrix, cameraViewportMatrix, BLACK);

		// 線分は両端をそれぞれスクリーン座標系まで変換し、Novice::DrawLineを利用して描画する
		Vector3 start = viewFinilTransform(viewFinilTransform(segment.origin, cameraWorldViewProjectionMatrix), cameraViewportMatrix);
		Vector3 end = viewFinilTransform(viewFinilTransform(Add(segment.origin, segment.diff), cameraWorldViewProjectionMatrix), cameraViewportMatrix);
		Novice::DrawLine(int(start.x), int(start.y), int(end.x), int(end.y), WHITE);


		// pointやsegmentの値もImGuiで編集できるようにして結果を動かして理解を深めると良い

		ImGui::Begin("rotate");
		//ImGui::SliderFloat3("cameraTranslate", &cameraTranslate.x, -10.0f, 10.0f);
		//ImGui::SliderFloat3("cameraRotate", &cameraRotate.x, -3.14f, 3.14f);
		//ImGui::SliderFloat3("point", &point.x, -10.0f, 10.0f);
		
		/// 点から線分への正射影ベクトルの結果はImGuiを利用して表示する
		ImGui::InputFloat3("Point", &point.x, "%.3f");
		ImGui::InputFloat3("Segment origin", &segment.origin.x, "%.3f");
		ImGui::InputFloat3("Segment diff", &segment.diff.x, "%.3f");
		//ImGui::SliderFloat3("Point", &point.x, -10.0f, 10.0f);
		//ImGui::SliderFloat3("Segment origin", &segment.origin.x, -10.0f, 10.0f);
		//ImGui::SliderFloat3("Segment diff", &segment.diff.x, -10.0f, 10.0f);
		ImGui::InputFloat3("Project", &project.x, "%.3f", ImGuiInputTextFlags_ReadOnly);

		ImGui::End();

		/// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
		/// StageEND
		/// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
