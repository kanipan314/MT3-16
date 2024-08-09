#include <Novice.h>
#include "MT.h"
#include "imgui.h"

const char kWindowTitle[] = "学籍番号";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	AABB aabb = { {-0.5f, -0.5f, -0.5f}, {0.5f ,0.5f, 0.5f} };
	Segment segment{ {0.7f, 0.3f, 0.0f}, {2.0f, -0.5f, 0.0f} };
	uint32_t color = WHITE;

	Vector3 rotate = {};
	Vector3 translate = {};
	Vector3 cameraRotate = { 0.0f, 0.0f, 0.0f };
	Vector3 cameraTranslate = { 0.0f, 0.0f, -9.49f };
	Vector3 cameraPosition = { 0,0,-5.0f };

	Vector3 start{};
	Vector3 end{};

	Matrix4x4 viewProjectionMatrix = {};
	Matrix4x4 viewportMatrix = {};
	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///
		viewProjectionMatrix = MakeViewProjectionMatrix({ 1, 1, 1 }, rotate, translate, { 1, 1, 1 }, cameraRotate, cameraTranslate);
		viewportMatrix = MakeViewportMatrix(0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);

		if (IsCollision(aabb, segment)) {
			color = RED;
		}
		else {
			color = WHITE;
		}


		ImGui::DragFloat3("segment.origin", &segment.origin.x, 0.01f);
		ImGui::DragFloat3("segment.diff", &segment.diff.x, 0.01f);
		ImGui::DragFloat3("aabb.min", &aabb.min.x, 0.01f);
		ImGui::DragFloat3("aabb.max", &aabb.max.x, 0.01f);
		ImGui::DragFloat3("cameraRotate", &cameraRotate.x, 0.01f);
		ImGui::DragFloat3("cameraTranslate", &cameraTranslate.x, 0.01f);

		aabb.min.x = (std::min)(aabb.min.x, aabb.max.x);
		aabb.max.x = (std::max)(aabb.min.x, aabb.max.x);
		aabb.min.y = (std::min)(aabb.min.y, aabb.max.y);
		aabb.max.y = (std::max)(aabb.min.y, aabb.max.y);
		aabb.min.z = (std::min)(aabb.min.z, aabb.max.z);
		aabb.max.z = (std::max)(aabb.min.z, aabb.max.z);

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		DrawSegment(segment, viewProjectionMatrix, viewportMatrix, color);
		DrawAABB(aabb, viewProjectionMatrix, viewportMatrix, WHITE);
		DrawGrid(viewProjectionMatrix, viewportMatrix);

		/// 
		/// ↑描画処理ここまで
		///

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
