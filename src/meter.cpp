#include "meter.h"

#include <jni.h>
#include<android/log.h>


#define LOG_TAG "jolyW"
#define LOGE(fmt, args...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, fmt, ##args)

using namespace kanzi;


class Coagent;
static Coagent* gApp = KZ_NULL;


Coagent::Coagent()
{
	m_pMsgQue = new SyncQueue<CommMsg>(512);
	m_isProjectLoad = false;
}
	
Coagent::~Coagent(){
	if (m_pMsgQue)
	{
		delete m_pMsgQue;
		m_pMsgQue = NULL;
	}
}
	
void Coagent::onProjectLoaded()
{
	m_isProjectLoad = true;
	
	ScreenSharedPtr screen = getScreen();
	mPage = getScreen()->lookupNode<Page>("#RootPage");
}
	
void Coagent::onConfigure(ApplicationProperties& configuration)
{
	configuration.binaryName = "./Meter.kzb";
}
void Coagent::onUpdate(kanzi::chrono::milliseconds deltaTime)
{
	static int sFramesCount = 0;
	
	if (!m_isProjectLoad)
	{
		return;
	}
	
	auto dataNode = mPage->lookupNode<Node2D>("#DataNode");
	sFramesCount += 1;
	m_valueFPS += (int)kzsICeilf(kzuEngineGetFramesPerSecond(m_engine));
	if (sFramesCount >= 100)
	{
		dataNode->setProperty(DynamicPropertyType<int>("FPSValue"),m_valueFPS/100);
		m_valueFPS = sFramesCount = 0;
	}
	
	handlerMsgLoop();
	kanzi::Application::onUpdate(deltaTime);
}

void Coagent::registerMetadataOverride(ObjectFactory& /*factory*/)
{
	Domain* domain = getDomain();
	domain->registerModule<KanziComponentsModule>("kanzi_components");
}
void Coagent::putMsgToKanzi(const CommMsg& msg)//对外接口
{
	//LOGE("[%s] >> enter.",__FUNCTION__);
	m_pMsgQue->TryPut(msg);
}
	
void Coagent::handlerMsgLoop()
{
	while (true)
	{
		CommMsg msg;
		bool ret = m_pMsgQue->TryTake(msg);
		if (ret)
		{
			handlerMsg(msg);
		}
		else
		{
			break;
		}

	}
}
	
void Coagent::handlerMsg(CommMsg& msg)
{
	//LOGE("[%s] >> enter.",__FUNCTION__);
   // LOGE("msg.m_msgType:",msg.m_msgType);
		
	//找到数据节点， 所有的数据都设置在这个节点上。
	auto dataNode = mPage->lookupNode<Node2D>("#DataNode");
	if (!dataNode)
	{
		LOGE("data node not found!");
		return;
	}
	
	
	
	switch(msg.m_msgType){
		case MSG_ID_ContentType://中间内容显示：0 车模显示 1 蓝牙电话 2 音乐 3 蓝牙音乐 4 导航 5 车模报警 6 导航飞屏动画
			dataNode->setProperty(DynamicPropertyType<int>("ContentType"),msg.intValue);break;
		case MSG_ID_TurnLeftLight://左转向灯亮
			dataNode->setProperty(DynamicPropertyType<bool>("TurnLeftLight"),msg.bool_value);break;
		case MSG_ID_TurnRightLight://右转向灯亮
			dataNode->setProperty(DynamicPropertyType<bool>("TurnRightLight"),msg.bool_value);break;
		case MSG_ID_RotateValue://转速
			dataNode->setProperty(DynamicPropertyType<std::string>("RotateValue"),msg.Stringvalue);break;
		case MSG_ID_SpeedValue://车速
			dataNode->setProperty(DynamicPropertyType<std::string>("SpeedValue"),msg.Stringvalue);break;
		case MSG_ID_SongName://歌曲名
			dataNode->setProperty(DynamicPropertyType<std::string>("SongName"),msg.Stringvalue);break;
		case MSG_ID_SingerName://歌手
			dataNode->setProperty(DynamicPropertyType<std::string>("SingerName"),msg.Stringvalue);break;
		case MSG_ID_TurnRoundDistance://顶部三个变量
			dataNode->setProperty(DynamicPropertyType<std::string>("TurnRoundDistance"),msg.Stringvalue);break;
		case MSG_ID_TargetDistance:
			dataNode->setProperty(DynamicPropertyType<std::string>("TargetDistance"),msg.Stringvalue);break;
		case MSG_ID_TopTime:
			dataNode->setProperty(DynamicPropertyType<std::string>("TopTime"),msg.Stringvalue);break;
		case MSG_ID_DTEDistance:
			dataNode->setProperty(DynamicPropertyType<std::string>("DTEDistance"),msg.Stringvalue);break;
		case MSG_ID_BatteryLevel:   //电池level 0 -10 共十一个值
				dataNode->setProperty(DynamicPropertyType<int>("BatteryLevel"),msg.intValue);break;
		case MSG_ID_ODODistance://总里程
			dataNode->setProperty(DynamicPropertyType<std::string>("ODODistance"),msg.Stringvalue);	break;
		case MSG_ID_GearType:  //档位 0 - 4 对应：P R N D S
			dataNode->setProperty(DynamicPropertyType<int>("GearType"),msg.intValue);
			break;
		case MSG_ID_CallName://电话人名
			dataNode->setProperty(DynamicPropertyType<std::string>("CallName"),msg.Stringvalue);
			break;
		case MSG_ID_CallState://电话状态
			dataNode->setProperty(DynamicPropertyType<std::string>("CallState"),msg.Stringvalue);
			break;
			
		case MSG_ID_Carc_doorFL://左前门：    
			dataNode->setProperty(DynamicPropertyType<int>("Carc_doorFL"),msg.intValue);
			break;
		case MSG_ID_Carc_doorBL://左后门：    
			dataNode->setProperty(DynamicPropertyType<int>("Carc_doorBL"),msg.intValue); 
			break;
		
		case MSG_ID_Carc_doorBR://右后门：    
			dataNode->setProperty(DynamicPropertyType<int>("Carc_doorBR"),msg.intValue);
			break;
		case MSG_ID_Carc_doorFR://右前门：    
			dataNode->setProperty(DynamicPropertyType<int>("Carc_doorFR"),msg.intValue);
			break;
		
		case MSG_ID_MusicState:   //播放状态
			dataNode->setProperty(DynamicPropertyType<bool>("MusicState"),msg.bool_value);
			break;
			
		case MSG_ID_ABSLight:	//防抱死故障指示灯
			dataNode->setProperty(DynamicPropertyType<bool>("ABSLight"),msg.bool_value);
			break;
			
		case MSG_ID_AirBagLight:	//安全气囊
			dataNode->setProperty(DynamicPropertyType<bool>("AirBagLight"),msg.bool_value);
			break;
			
		case MSG_ID_BatteryLight:	//电池电量过低
			dataNode->setProperty(DynamicPropertyType<bool>("BatteryLight"),msg.bool_value);
			break;
			
		case MSG_ID_BrakePadLight:	//刹车片磨损指示灯
			dataNode->setProperty(DynamicPropertyType<bool>("BrakePadLight"),msg.bool_value);
			break;
		
		case MSG_ID_BrakePedalNotDePress:	//制动踏板未踩下指示灯
			dataNode->setProperty(DynamicPropertyType<bool>("BrakePedalNotDePress"),msg.bool_value);
			break;
		
		case MSG_ID_CarDoorLight:	//车门未关
			dataNode->setProperty(DynamicPropertyType<bool>("CarDoorLight"),msg.bool_value);
			break;
		
		case MSG_ID_DippedHeadLight:	//前照灯
			dataNode->setProperty(DynamicPropertyType<bool>("DippedHeadLight"),msg.bool_value);
			break;
		
		case MSG_ID_EngineWarmUpLight:	//发动机预热指示灯
			dataNode->setProperty(DynamicPropertyType<bool>("EngineWarmUpLight"),msg.bool_value);
			break;
		
		case MSG_ID_EPCLight:	//发动机电子稳定系统指示灯
			dataNode->setProperty(DynamicPropertyType<bool>("EPCLight"),msg.bool_value);
			break;
		
		case MSG_ID_ESPLight:	//车身电子稳定系统指示灯
			dataNode->setProperty(DynamicPropertyType<bool>("ESPLight"),msg.bool_value);
			break;
		
		case MSG_ID_FogFrontLight:	//前雾灯
			dataNode->setProperty(DynamicPropertyType<bool>("FogFrontLight"),msg.bool_value);
			break;
		
		case MSG_ID_FogRearLight:	//后雾灯
			dataNode->setProperty(DynamicPropertyType<bool>("FogRearLight"),msg.bool_value);
			break;
		
		case MSG_ID_HeatingFrontLight:	//前窗加热指示灯
			dataNode->setProperty(DynamicPropertyType<bool>("HeatingFrontLight"),msg.bool_value);
			break;
		
		case MSG_ID_HeatingRearLight:	//后窗加热指示灯
			dataNode->setProperty(DynamicPropertyType<bool>("HeatingRearLight"),msg.bool_value);
			break;
		
		case MSG_ID_ImmobiliserLight:	//发动机防盗锁止系统灯
			dataNode->setProperty(DynamicPropertyType<bool>("ImmobiliserLight"),msg.bool_value);
			break;
		
		case MSG_ID_LowCleanWaterLight:		//清洗液液位低指示灯
			dataNode->setProperty(DynamicPropertyType<bool>("LowCleanWaterLight"),msg.bool_value);
			break;
		
		case MSG_ID_LowFuelLight:	//燃油低指示灯
			dataNode->setProperty(DynamicPropertyType<bool>("LowFuelLight"),msg.bool_value);
			break;
		
		case MSG_ID_LowOilPressure:	//机油压力低指示灯
			dataNode->setProperty(DynamicPropertyType<bool>("LowOilPressure"),msg.bool_value);
			break;
		
		case MSG_ID_LowPressureLight:	//胎压过低指示灯
			dataNode->setProperty(DynamicPropertyType<bool>("LowPressureLight"),msg.bool_value);
			break;
		
		case MSG_ID_MotorDetectLight:	//发动机故障指示灯
			dataNode->setProperty(DynamicPropertyType<bool>("MotorDetectLight"),msg.bool_value);
			break;
		
		case MSG_ID_ParkingBrake:	//驻车故障指示灯
			dataNode->setProperty(DynamicPropertyType<bool>("ParkingBrake"),msg.bool_value);
			break;
		
		case MSG_ID_SafetyLight:  //安全带未系指示灯
			dataNode->setProperty(DynamicPropertyType<bool>("SafetyLight"),msg.bool_value);
			break;
		
		case MSG_ID_SteeringWheelLight:	//转向助力系统故障指示灯
			dataNode->setProperty(DynamicPropertyType<bool>("SteeringWheelLight"),msg.bool_value);
			break;
		
		case MSG_ID_WidthLight:  //示宽灯
			dataNode->setProperty(DynamicPropertyType<bool>("WidthLight"),msg.bool_value);
			break;	
			
		case MSG_ID_AnimationStart:  //动画启动开关
			dataNode->setProperty(DynamicPropertyType<bool>("bStart"),msg.bool_value);
			break;	
			
		case MSG_ID_iMusicMoveState:  //音乐动画，1 音乐切换到蓝牙电话 2 蓝牙电话切换到音乐 3 音乐飞屏动画
			dataNode->setProperty(DynamicPropertyType<int>("iMusicMoveState"),msg.intValue);
			break;	
			
		case MSG_ID_bMachineStart:  //组合屏开机动画，0 暂停播放，切回仪表界面  1 播放（时间3s）
			dataNode->setProperty(DynamicPropertyType<int>("bMachineStart"),msg.intValue);
			break;
			
		case MSG_ID_TopContentType:  //顶部内容显示：0 蓝牙来电 1 蓝牙去电 2 音乐 3 导航
			dataNode->setProperty(DynamicPropertyType<int>("TopContentType"),msg.intValue);
			break;
			
		case MSG_ID_sNaviDirection:  //导航方向指示，按照百度地图图片名
			dataNode->setProperty(DynamicPropertyType<std::string>("sNaviDirection"),msg.Stringvalue);
			break;
			
		case MSG_ID_sTimeValue:  //当前时间值，例如 9:00 
			dataNode->setProperty(DynamicPropertyType<std::string>("sTimeValue"),msg.Stringvalue);
			break;
			
		case MSG_ID_iTimePeriod:  //当前时间段，0：上午（am），1：下午（pm）
			dataNode->setProperty(DynamicPropertyType<int>("iTimePeriod"),msg.intValue);
			break;
	
		
	}
	
}

void Coagent::mediaGraphShow(std::string node, std::string pathfile)
{
	KzcImage* image =NULL;
	const std::string nodePath = "#MusicListItem";
	
	char charPath[ MAX_PATH ] = {0};
	snprintf( charPath, MAX_PATH, "%s", pathfile.c_str());
			
	kzcImageLoadFileFlipped(getDomain()->getMemoryManager(),charPath,&image);
	if (image){
		kzcImagePremultiplyAlpha(image);
		TextureSharedPtr newTexture = Texture::create(getDomain(),image,0,"image");

		if(newTexture)
		{
			NodeSharedPtr targetNode = mPage->lookupNode<Node>(nodePath.c_str());
			if(targetNode)
			{
					targetNode->setProperty(StandardMaterial::TextureProperty, newTexture); 
			}
			else{
				//LOGE("kanzi error	not find Path:%s\n", path.c_str());
			}
		}
	}
	else{
		//LOGE("Error Cannot find image:%s\n", charPath);
	}
}


	
Application* createApplication()
{
	gApp = new Coagent();
	return gApp;
}  

#if defined(__cplusplus)
extern "C" {
#endif 

JNIEXPORT void JNICALL Java_com_rightware_kanzi_car_CarDemo_sendMsgStringToKanzi(JNIEnv *env, jobject obj, jint msgType,jstring value)
{
         //LOGE("[%s] >> speed= %d.\n",__FUNCTION__, speed);
		 CommMsg commMsg;
		 commMsg.m_msgType = msgType;
		 commMsg.Stringvalue = env->GetStringUTFChars(value,false);
		 gApp->putMsgToKanzi(commMsg);
}

JNIEXPORT void JNICALL Java_com_rightware_kanzi_car_CarDemo_sendMsgIntToKanzi(JNIEnv *env, jobject obj, jint msgType,jint value)
{
         //LOGE("[%s] >> speed= %d.\n",__FUNCTION__, speed);
		 CommMsg commMsg;
		 commMsg.m_msgType = msgType;
		 commMsg.intValue = value;
		 gApp->putMsgToKanzi(commMsg);
}

JNIEXPORT void JNICALL Java_com_rightware_kanzi_car_CarDemo_sendMsgBoolToKanzi(JNIEnv *env, jobject obj, jint msgType,jboolean value)
{
         //LOGE("[%s] >> speed= %d.\n",__FUNCTION__, speed);
		 CommMsg commMsg;
		 commMsg.m_msgType = msgType;
		 commMsg.bool_value = value;
		 gApp->putMsgToKanzi(commMsg);
}

#if defined(__cplusplus)
}
#endif

