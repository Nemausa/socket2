#include"Log.hpp"
#include"Config.hpp"
#include"GroupServer.hpp"
#include"DBUser.hpp"


using namespace doyou::io;

void sqlite_test()
{
	////////////////////
	DBUser db;
	db.init();
	int count = 100000;
	int nMax = 10;
	char username[32] = {};
	char nickname[32] = {};

	Timestamp timestamp00;
	//事务开始
	db.execDML("begin;");
	////////
	CELLLog_Info("db.add_user");
	for (int n = 0; n < nMax; n++)
	{
		Timestamp timestamp;
		for (int i = 0; i < count; i++)
		{
			sprintf(username, "test%05d", db.makeId());
			sprintf(nickname, "abc%05d", db.makeId());
			if (db.add_user(username, "mm123456", nickname, i % 2) > 0)
			{
				//CELLLog_Info(username);
			}
		}
		auto t = timestamp.getElapsedSecond();
		CELLLog_Info("%d   count=%d,  time=%f,      %f,      %f", n, count, t, t / count, count / t);
	}
	CELLLog_Info("db.add_user %f", timestamp00.getElapsedSecond());
	CELLLog_Info("#####################");
	CELLLog_Info("db.hasByKV");
	timestamp00.update();
	for (int n = 0; n < nMax; n++)
	{
		Timestamp timestamp;
		for (int i = 0; i < count; i++)
		{
			sprintf(username, "test%05d", 100000 + i + 1 + (n*count));
			if (db.hasByKV("user_info", "username", username))
			{
				//CELLLog_Info(username);
			}
		}
		auto t = timestamp.getElapsedSecond();
		CELLLog_Info("%d   count=%d,  time=%f,      %f,      %f", n, count, t, t / count, count / t);
	}
	CELLLog_Info("db.hasByKV %f", timestamp00.getElapsedSecond());
	CELLLog_Info("#####################");
	CELLLog_Info("db.findByKV");
	for (int n = 0; n < nMax; n++)
	{
		Timestamp timestamp;
		for (int i = 0; i < count; i++)
		{
			sprintf(username, "test%05d", 100000 + i + 1 + (n*count));
			neb::CJsonObject ret;
			if (db.findByKV("password,nickname", "user_info", "username", username, ret))
			{
				//CELLLog_Info(username);
			}
		}
		auto t = timestamp.getElapsedSecond();
		CELLLog_Info("%d   count=%d,  time=%f,      %f,      %f", n, count, t, t / count, count / t);
	}
	CELLLog_Info("db.findByKV %f", timestamp00.getElapsedSecond());
	CELLLog_Info("#####################");
	CELLLog_Info("db.updateByKV");
	timestamp00.update();
	int mm = Time::system_clock_now() % 100000;
	for (int n = 0; n < nMax; n++)
	{
		Timestamp timestamp;
		for (int i = 0; i < count; i++)
		{
			sprintf(username, "test%05d", 100000 + i + 1 + (n*count));
			sprintf(nickname, "abc%05d", mm);
			if (db.updateByKV("user_info", "username", username, "password", nickname))
			{
				//CELLLog_Info("%s >> %s", username, nickname);
			}
		}
		auto t = timestamp.getElapsedSecond();
		CELLLog_Info("%d   count=%d,  time=%f,      %f,      %f", n, count, t, t / count, count / t);
	}
	CELLLog_Info("db.updateByKV %f", timestamp00.getElapsedSecond());
	CELLLog_Info("#####################");
	CELLLog_Info("db.deleteByKV");
	timestamp00.update();
	for (int n = 0; n < nMax; n++)
	{
		Timestamp timestamp;
		for (int i = 0; i < count; i++)
		{
			sprintf(username, "test%05d", 100000 + i + 1 + (n*count));
			if (db.deleteByKV("user_info", "username", username))
			{
				//CELLLog_Info(username);
			}
		}
		auto t = timestamp.getElapsedSecond();
		CELLLog_Info("%d   count=%d,  time=%f,      %f,      %f", n, count, t, t / count, count / t);
	}
	CELLLog_Info("db.deleteByKV %f", timestamp00.getElapsedSecond());
	//提交事务
	db.execDML("commit;");
}

int main(int argc, char* args[])
{

#if _WIN32 && _CONSOLE
	system("chcp 65001");
#endif // _WIN32 && _CONSOLE

	//设置运行日志名称
	Log::Instance().setLogPath("GroupServerLog", "w", false);
	Config::Instance().Init(argc, args);
	///////////////////
	GroupServer server;
	server.Init();
	while (true)
	{
		server.Run();
	}
	server.Close();

	////在主线程中等待用户输入命令
	//while (true)
	//{
	//	char cmdBuf[256] = {};
	//	scanf("%s", cmdBuf);
	//	if (0 == strcmp(cmdBuf, "exit"))
	//	{
	//		server.Close();
	//		break;
	//	}
	//	else {
	//		CELLLog_Info("undefine cmd");
	//	}
	//}

	CELLLog_Info("exit.");

	return 0;
}
