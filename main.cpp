#include <Novice.h>
#include "myMath.h"
#include "crashDecision.h"
#include "ImGui.h"

const char kWindowTitle[] = "GC01_05_カン_ケンリャン";

int kWindowWidth = 1280, kWindowHeight = 720;

///=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
/// 重要定義、構造体作り
///=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

/// 計算に使う値
Segment segment{ { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 2.0f} };
Plane plane{ { 0.0f, 1.0f, 0.0f }, 1.0f };

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, kWindowWidth, kWindowHeight);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	///=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
	/// 初期化
	///=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=


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

		if (keys[DIK_A]) { cameraTranslate.x -= 0.1f; }
		if (keys[DIK_D]) { cameraTranslate.x += 0.1f; }
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

		///// 判定と描画
		if (crashDecision(segment, plane)) {
			Draw3DSegment(segment, cameraWorldViewProjectionMatrix, cameraViewportMatrix, 0xFF0000FF);
		} else {
			Draw3DSegment(segment, cameraWorldViewProjectionMatrix, cameraViewportMatrix, 0xFFFFFFFF);
		}
		DrawPlane(plane, cameraWorldViewProjectionMatrix, cameraViewportMatrix, 0xFFFFFFFF);

		/// ImGui
		ImGui::Begin("Balls");
		ImGui::DragFloat3("Plane.Normal", &plane.normal.x, 0.1f);
		ImGui::DragFloat("Plane.Distance", &plane.distance, 0.1f);
		ImGui::SliderFloat3("Segment.Origin", &segment.origin.x, -5.0f, 5.0f);
		ImGui::SliderFloat3(" Segment.Diff", &segment.diff.x, -5.0f, 5.0f);
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
