#include "ProjectConfig.h"
#include "DxLib.h"

// デバックフラグの初期化
bool ProjectConfig::DEBUG = false;

// ステージサイズの初期化
int ProjectConfig::STAGE_WIDTH = 3840;
int ProjectConfig::STAGE_HEIGHT = 768;

// エラー内容を出力する
int ErrorThrow(std::string error_log)
{
	// Log.txtにエラー内容を追加する
	ErrorLogFmtAdd(error_log.c_str());
	return D_FAILURE;
}