#ifndef COAGENT_H_
#define COAGENT_H_

 
#include <kanzi/kanzi.hpp>
#include <system/file/kzs_file.h>
#include <system/wrappers/kzs_math.h>

#include "SyncQueue.h"
#include "commtype.h"

#define LOG_TAG "jolyW"
#define LOGE(fmt, args...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, fmt, ##args)
#define MAX_PATH  1024

using namespace kanzi;

class Coagent :public kanzi::Application
{
public:
	Coagent();

	virtual ~Coagent();
	virtual void onProjectLoaded();
	
	virtual void onConfigure(ApplicationProperties& configuration);
	
	virtual void onUpdate(kanzi::chrono::milliseconds deltaTime);
	
	void registerMetadataOverride(ObjectFactory& /*factory*/);
	
	void putMsgToKanzi(const CommMsg& msg);//对外接口
	
	void handlerMsgLoop();
	
	void handlerMsg(CommMsg& msg);

	void mediaGraphShow(std::string node, std::string fielPath);
	
	
private:
	bool m_isProjectLoad;
	SyncQueue<CommMsg>* m_pMsgQue;
	EmptyNode3DSharedPtr m_pNode3d;
	PageSharedPtr   mPage;
	int	 m_valueFPS;
};


#endif /* COAGENT_H_ */
