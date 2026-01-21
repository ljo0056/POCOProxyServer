#pragma once

#pragma warning(disable: 4200) 
typedef struct _CHROZEN_PACKET
{
    unsigned int nPacketLength;         // 전체 길이
    unsigned int nDeviceID;             // reserved for future, must be 0
    unsigned int nPacketCode;
    unsigned int nEventIndex;           // Event일 때의 Index
    unsigned int nSlotOffset;
    unsigned int nSlotSize;             // slot's size
    unsigned char Slot[0];              // warning 4200
} CHROZEN_PACKET;
#pragma warning(default: 4200) 

const int PACKET_DEVICE_ID_YLAS = (0x97000);

// 2020-03-03 jolee
// [주의사항]
//   - Packet code 추가 시에 class PacketConfig 의 맴버 함수 IsIncludePacketCode 에 해당 code 사용 유무를 체크 해야함
enum CHROZEN_PACKET_CODE
{
	PACKCODE_SYSTEM_INFORM = (PACKET_DEVICE_ID_YLAS + 0x400),   // 0x 00 09 74 00
	PACKCODE_SYSTEM_CONFIG = (PACKET_DEVICE_ID_YLAS + 0x410),
	PACKCODE_RUN_SETUP     = (PACKET_DEVICE_ID_YLAS + 0x420),
	PACKCODE_TIME_EVENT    = (PACKET_DEVICE_ID_YLAS + 0x430),
	PACKCODE_MIX_SETUP     = (PACKET_DEVICE_ID_YLAS + 0x440),
	PACKCODE_SEQUENCE      = (PACKET_DEVICE_ID_YLAS + 0x460),
	PACKCODE_COMMAND       = (PACKET_DEVICE_ID_YLAS + 0x470),
	PACKCODE_STATE         = (PACKET_DEVICE_ID_YLAS + 0x480),
	PACKCODE_SLFEMSG       = (PACKET_DEVICE_ID_YLAS + 0x490),
	PACKCODE_SERVICE       = (PACKET_DEVICE_ID_YLAS + 0x500),
	PACKCODE_ADJUST_DATA   = (PACKET_DEVICE_ID_YLAS + 0x510),
	PACKCODE_DIAG_DATA     = (PACKET_DEVICE_ID_YLAS + 0x520),
	PACKCODE_USED_TIME     = (PACKET_DEVICE_ID_YLAS + 0x540),
	PACKCODE_UPDATE        = (PACKET_DEVICE_ID_YLAS + 0x700),
	PACKCODE_SPECIAL       = (PACKET_DEVICE_ID_YLAS + 0x620),
};



typedef struct _TIME_t
{
	unsigned char hour;
	unsigned char minute;
	unsigned char second;
} TIME_t;

typedef struct _DATE_t
{
	unsigned char year;
	unsigned char month;
	unsigned char date;
} DATE_t;

typedef struct _SYSTEM_INFORM_t
{
	char cModel[32];		// 기기 정보 ex) "LC AutoSampler"
	char cVersion[9];		// 버전		 ex) "1.0.0"

	char cSerialNo[17];		// 시리얼번호 - Serial No ex) AS 2345 161031 7654 --> AS 2345 161031 1DE6 
	// 1DE6 -> 9999-2345 = 7654 (Hex 1DE6)
	DATE_t cInstallDate;	// 인스톨날짜	 ex) "20170306"

	DATE_t cSysDate;		// ex) "20170306"
	TIME_t cSysTime;			// ex) "190630"

	char cIPAddress[16];	// ex) "10.10.10.52"
	char cPortNo[5];			// ex) "4242"

    CString GetModel()
    {
        CString str(cModel);
        str.TrimLeft();
        str.TrimRight();
        return str;
    }

	CString GetSerialNumber()
	{
		char temp[18];
        memcpy(temp, cSerialNo, 17);
        temp[17] = 0;

		return CString(temp);
	}

    CString GetVersion()
    {
        CString str(cVersion);
        str.TrimLeft();
        str.TrimRight();
        return str;
    }

	_SYSTEM_INFORM_t()
	{
		memset(this, 0, sizeof(struct _SYSTEM_INFORM_t));
		//strcpy_s<32>(cModel, "Chrozen Autosampler");  // 2020-10-28 jolee   model default 이름 변경 시에 0 Byte 만 수신됨
        strcpy_s<32>(cModel, "YL9152 LC AutoSampler");
		strcpy_s<9>(cVersion, "1.0.0");
	}
} SYSTEM_INFORM_t;


enum Syringes
{
	Syringe250, Syringe500, Syringe1000, Syringe2500
};
enum Trays
{
	Tray40Vial,
	Tray60Vial,
	Tray96Plate,
	TrayNone
};
enum ProcessTypes
{
	RowFirst, ColumnFirst
};

enum PulseTimes
{
	NoPulse,
	Pulse100ms,
	Pulse200ms,
	Pulse500ms,
	Pulse1sec,
	Pulse2sec
};

typedef struct _SYSTEM_CONFIG_t
{
	unsigned int trayCooling;		// ( 0:Not install , 1:Install)

	unsigned int sampleLoopVol;  	// 0000 - 10000 uL	, During Prep Mode the loop volume is fixed at 10000uL
	unsigned int syringeVol;		// 0 : 250uL  1: 500uL(default)  2:1000uL 3: 2500uL in Prep Mode
	unsigned int needleTubeVol;		// needle Tubing 용량(ul) : 250uL(default) 	// 000 - 1000 uL

	unsigned int trayLeft;		    // left(back) tray
	unsigned int trayRight;		    // right(front) tray
	// 0 = 40 VIALS
	// 1 = 60 VIALS
	// 2 = 96 VIALS
	// 3 = None

	unsigned int processType;	// 주입진행순서
	// 0 = ROW
	// 1 = COLUMN

	unsigned int syringeUpSpdWash;	// default:(6)  (1~10) 1:LOW			10:HIGH
	unsigned int syringeDnSpdWash;	// default:(6)  (1~10) 1:LOW			10:HIGH

	unsigned int injectMarkerTime;	// 스타트 시그널 출력 시간 (ms) ,	100ms ~ 2000ms (100ms default)
	unsigned int startInSigTime;	// 스타트시그널 인식 최소시간 (default : 입력받지 않음)

	unsigned char useMicroPump;

	_SYSTEM_CONFIG_t()
	{
		memset(this, 0, sizeof(struct _SYSTEM_CONFIG_t));
		sampleLoopVol = 20;
		needleTubeVol = 250;
		processType = ColumnFirst;
		syringeUpSpdWash = 6;
		syringeDnSpdWash = 6;
		injectMarkerTime = Pulse100ms;
        syringeVol = 1;
	}
} SYSTEM_CONFIG_t;



typedef struct _LCAS_RUN_SETUP_t
{
	enum InjectionModes
	{
		FullLoop,		// 0 = Full loop injection mode					: 100uL
		PartialLoop,	// 1 = Partial loop fill injection mode	: 0-50uL	// Sample loop의 1/2까지 가능
		Pickup			// 2 = μL pick up injection mode				: 0-27uL
	};

	enum InjectionStarts
	{
		AnyTime,		// 0 - anytime : 스타트 명령(신호)를 받으면 즉시 시작 
		RemoteReady,	// 1 - remote ready
		TempReady,		// 2 - temp ready : trayCooling 사용하고 temp on시 적용 온도가 안정되면 시작 : 온도 off시에는 즉시 시작
		AllReady		// 3 - all ready : remote & temp ready

	};

	enum WashBetweens
	{
		NoWash,
		BetweenInjections,
		BetweenVials,
		BetweenSequences
	};

	unsigned char tempOnoff;			// SYSTEM_CONFIG_t의 trayCooling 설정에 따라 사용유무 적용
	int temp;						// Tray 설정온도 (4도 ~ 60도)
	unsigned char injectionStart;		// 사용안함.
	// 0 - anytime : 스타트 명령(신호)를 받으면 즉시 시작 
	// 1 - remote ready
	// 2 - temp ready : trayCooling 사용하고 temp on시 적용 온도가 안정되면 시작
	//	: 온도 off시에는 즉시 시작
	// 3 - all ready : remote & temp ready

	unsigned char injectionMode;
	// 0 = Full loop injection mode					: 100uL
	// 1 = Partial loop fill injection mode	: 0-50uL	// Sample loop의 1/2까지 가능 
	// 2 = μL pick up injection mode				: 0-27uL

	float analysisTime;		// minute

	unsigned char flushEnable;
	float flushVolume;		// FLUSH_VOLUME_DEFAULT

	float injectionVolume; 	// 실린지용량의 1/5 보다 작게  0~500
	// 실린지 설정에 따라 최대값(실린지용량의 1/5)이 달라진다.
	unsigned char washBetween;	// 세척간격 
	// 0 = NONE			//
	// 1 = BETWEEN INJECTIONS
	// 2 = BETWEEN VIALS
	// 3 = ANALYSIS END
	unsigned int washVolume;			// 세척량 - 최대 : 시린지볼륨

	// 세척횟수(0~9) - Mix 와 sample injection에서 동일하게 적용(동작)
	unsigned char washNeedleBeforeInj;		// default 0 - needle 및 needle tubing내부
	unsigned char washNeedleAfterInj;			// default 1
	unsigned char washInjportBeforeInj;		// default 0 - 인젝터 세척 횟수
	unsigned char washInjportAfterInj;		// default 1

	unsigned char syringeUpSpdInj;	// default:(6)  (1~10) 1:LOW			10:HIGH
	unsigned char syringeDnSpdInj;	// default:(6)  (1~10) 1:LOW			10:HIGH
	float needleHeight;	// 2~6mm(0.5mm단위) - 바이알(외부)의 밑부분에서 부터

	float waitTime_AfterAspirate;			// 샘플흡입후 대기시간 - 점도 있는 시료를 위한 (0~30sec)
	float waitTime_AfterDispense;			// 샘플배출후 대기시간 - 점도 있는 시료를 위한 (0~30sec)

	unsigned char skipMissingSample;	// 바이알이 없으면 selfMessage 전송
	// 					0 = NO		// error - send_SelfMessage(SELF_ERROR, SELF_ERROR_MISSING_VIAL_WAIT);
	// default 	1 = Skip missing sample position - send_SelfMessage(SELF_ERROR, SELF_ERROR_MISSING_VIAL_SKIP);

	unsigned char airSegment;	// default (0) : 0 = Not use
	// 1 = Use air segment
	float airSegmentVol;		//default(5uL) 	(1~20)

	//int reserved;
    unsigned char microPumpTime;	// sec(0: 1sec / 1: 2sec ~ 9: 10sec)
    unsigned char reserved1;
    unsigned char reserved2;
    unsigned char reserved3;

	_LCAS_RUN_SETUP_t()
	{
        void Init();
	}

    void Init()
    {
        memset(this, 0, sizeof(struct _LCAS_RUN_SETUP_t));

		injectionMode = PartialLoop;

        temp = 4;
		flushEnable = 1;
        flushVolume = 10;

        syringeUpSpdInj = 6;
        syringeDnSpdInj = 3;

        needleHeight = 2.0f;
        skipMissingSample = 1;

		waitTime_AfterAspirate = 0.2f;
		waitTime_AfterDispense = 0.2f;

		washBetween = BetweenInjections;
		washVolume = 500;

		washNeedleBeforeInj = 1;
		washNeedleAfterInj = 0;
		washInjportBeforeInj = 0;
		washInjportAfterInj = 1;

		microPumpTime = 2;

		airSegment = 1;
        airSegmentVol = 5;
    }

} LCAS_RUN_SETUP_t;

typedef struct _AUX_EVENT
{
	float fTime;				// 실행시간 [min]	- (0.0~10000) : 음수 끝
	unsigned char auxOnoff;		 // 0:OFF, 1:ON, // 2:TOGGLE 3: PULSE
	_AUX_EVENT() 
	{
        Init();
	}

    void Init()
    {
        fTime = -1.0f;
        auxOnoff = 0;
    }
} AUX_EVENT;

typedef struct _LCAS_TIME_EVENT_t
{
	unsigned char useAuxTimeEvent;	//0:사용하지 않음, 1:사용
	unsigned char initAux;			 // 0:OFF, 1:ON,

    enum
    {
        AUX_TABLE_COUNT = 20,
        AUX_TABLE_VAILD_COUNT = AUX_TABLE_COUNT - 1,
    };
	AUX_EVENT auxEvent[AUX_TABLE_COUNT];

	float endTime;					// 타임이벤트 종료시간 [min] - 마지막 상태유지
	unsigned char auxOutSigTime;	// pulse 출력시간 : 100ms ~ 2000ms (disable default)

	_LCAS_TIME_EVENT_t()
	{
        Init();
	}

    void Init()
    {
        useAuxTimeEvent = 0;
        initAux = 0;
        endTime = 0.f;
        auxOutSigTime = 0;

        for (int ii = 0 ; ii < AUX_TABLE_COUNT ; ii++)
            auxEvent[ii].Init();
    }
} LCAS_TIME_EVENT_t;




typedef struct _U_VIAL_POS_t
{
	unsigned char tray;			// 0: Left, 1: Right 2: 기타위치
	unsigned char etc;			// tray가 기타위치(2)일 경우 참조
	/*	#define POS_XY_INJECTOR			0
	#define POS_XY_WASTE				1
	#define POS_XY_WASH					2
	#define POS_XY_EXCHANGE			3
	#define POS_XY_HOME					4		*/
	unsigned char x;				// 0~5 - row(가로 행) (A~F)  			SYSTEM_CONFIG_t의 trayLeft설정(Tray의 종류)에 따라 최대가 달라짐
	unsigned char y;				// 0~11 - column(세로 열) (1~12) 	SYSTEM_CONFIG_t의 trayLeft설정(Tray의 종류)에 따라 최대가 달라짐

	CString GetCaption(const SYSTEM_CONFIG_t & config)
	{
		CString szCaption(_T("Unknown"));
		switch (tray)
		{
		case 0: 
			if (config.trayLeft != Tray96Plate)
				szCaption.Format(_T("L(%c%d)"), _T('A') + x, y + 1);
			else
				szCaption.Format(_T("B(%c%d)"), _T('A') + x, y + 1);
			break;
		case 1:
			if (config.trayRight != Tray96Plate)
				szCaption.Format(_T("R(%c%d)"), _T('A') + x, y + 1);
			else
				szCaption.Format(_T("F(%c%d)"), _T('A') + x, y + 1);
			break;
		case 2:
			switch (etc)
			{
			case 1:		szCaption = _T("Waste");	break;
			case 2:		szCaption = _T("Wash");		break;
			case 3:		szCaption = _T("Exchange");	break;
			case 4:		szCaption = _T("Home");		break;
			}
			break;
		}
		return szCaption;
	}
	bool IsValid(const SYSTEM_CONFIG_t & config)
	{
		switch (tray)
		{
		case 0:
			switch (config.trayLeft)
			{
			case Tray40Vial:			if (x > 3 || y > 9)		return false;	break;
			case Tray60Vial:			if (x > 4 || y > 11)	return false;	break;
			case Tray96Plate:			if (x > 7 || y > 11)	return false;	break;
			default:	return false;
			}
			break;
		case 1:
			switch (config.trayRight)
			{
			case Tray40Vial:			if (x > 3 || y > 9)		return false;	break;
			case Tray60Vial:			if (x > 4 || y > 11)	return false;	break;
			case Tray96Plate:			if (x > 7 || y > 11)	return false;	break;
			default:	return false;
			}
			break;
		case 2:
			if (etc > 4)	return false;
			break;
		default:
			return false;
		}
		return true;
	}
	bool operator == (const _U_VIAL_POS_t& vial)
	{
		if (tray == 2)
		{
			return etc == vial.etc;
		}
		return tray == vial.tray && x == vial.x && y == vial.y;
	}

	_U_VIAL_POS_t operator = (const WORD & vial)
	{
		tray = (vial >> 15) & 0x1;
		etc = (vial >> 12) & 0x07;
		if (etc != 0)	tray = 2;
		x = (vial >> 6) & 0x3f;
		y = vial & 0x3f;

		return *this;
	}
	operator WORD ()
	{
		return  ((WORD)(tray & 0x1) << 15) | ((WORD)(etc & 0x7) << 12) | ((WORD)(x & 0x3f) << 6) | (WORD)(y & 0x3f);
	}
private:
	int TotalVialCount(SYSTEM_CONFIG_t & config)
	{
		int total = 0;
		switch (config.trayLeft)
		{
		case Tray40Vial:			total += 40;	break;
		case Tray60Vial:			total += 60;	break;
		case Tray96Plate:			total += 96;	break;
		}
		switch (config.trayRight)
		{
		case Tray40Vial:			total += 40;	break;
		case Tray60Vial:			total += 60;	break;
		case Tray96Plate:			total += 96;	break;
		}
		return total;
	}
public:
	static int TotalVial(SYSTEM_CONFIG_t & config, CArray<_U_VIAL_POS_t> & arr)
	{
		_U_VIAL_POS_t v(0);
		for (v.tray = 0; v.tray < 2; v.tray++)
		{
			v.x = 0;
			v.y = 0;

			do {
				while (v.IsValid(config))
				{
					arr.Add(v);

					if (config.processType == RowFirst)
					{
						v.x++;
					}
					else
					{
						v.y++;
					}
				}
				if (config.processType == RowFirst)
				{
					v.x = 0;
					v.y++;
				}
				else
				{
					v.x++;
					v.y = 0;
				}
			} while (v.IsValid(config));
		}

		return (int)arr.GetSize();
	}


	_U_VIAL_POS_t OffsetTo(int off, SYSTEM_CONFIG_t & config)
	{
		int index = -1;
		CArray<_U_VIAL_POS_t> arr;
		TotalVial(config, arr);
		for (int i = 0; i < arr.GetSize(); i++)
			if (arr[i] == *this)	{
				index = i;	break;
			}

		if (index == -1)
			return *this;

		index += off;
		while (index < 0)
		{
			index += (int)arr.GetSize();
		}
		return arr[index % arr.GetSize()];

	}
	int OffsetFrom(_U_VIAL_POS_t vial, SYSTEM_CONFIG_t & config)
	{
		int index = -1;
		int indexBase = -1;
		CArray<_U_VIAL_POS_t> arr;
		TotalVial(config, arr);
		for (int i = 0; i < arr.GetSize(); i++)
		{
			if (arr[i] == *this)	index = i;
			if (arr[i] == vial)	indexBase = i;
		}
		if (index == -1 || indexBase == -1)
			return 0;

		return index - indexBase;
	}


	_U_VIAL_POS_t()
	{
		tray = etc = x = y = 0;
	}
	_U_VIAL_POS_t(const _U_VIAL_POS_t & p)
	{
		tray = p.tray;
		etc = p.etc;
		x = p.x;
		y = p.y;
	}
	_U_VIAL_POS_t(WORD vial)
	{
		*this = vial;
	}
} U_VIAL_POS_t;

typedef struct _VIAL_POS_t
{
	int pos;

	U_VIAL_POS_t GetVial()
	{
		U_VIAL_POS_t v;
		memcpy(&v, &pos, sizeof(v));
		return v;
	}
	void SetVial(const U_VIAL_POS_t & vial)
	{
		memcpy(&pos, &vial, sizeof(vial));
	}

	_VIAL_POS_t()
	{
		pos = 0;
	}
} VIAL_POS_t;


typedef struct _LCAS_MIX_TABLE_t
{
	unsigned char action;		// 테이블의 끝에는 action값을 MIX_ACTION_NONE으로 설정
	unsigned char addFrom;
	unsigned char addTo;
	float addVol;		// uL (0~Syringe Vol)

	unsigned char mixVial;	// 0: Sample    1: Destination --- 무조건 Destination
	unsigned char mixTimes;	// 1~9
	float mixVol;		// uL (0~Syringe Vol)

	unsigned short waitTime;	// sec (0 ~ MIX_WAIT_TIME_MAX)

	_LCAS_MIX_TABLE_t()
	{
		memset(this, 0, sizeof(struct _LCAS_MIX_TABLE_t));
	}
} LCAS_MIX_TABLE_t;

typedef struct _LCAS_MIX_t
{
	unsigned char useMixPrgm;		// 0 = not Use Mix Prgm		// 1 = Use Mix Prgm
	unsigned char relativeDestination;	// 0 - , 1 - relative destination	
	unsigned char relativeReagentA;
	unsigned char relativeReagentB;
	unsigned char relativeReagentC;
	unsigned char relativeReagentD;

	VIAL_POS_t firstDestPos;
	VIAL_POS_t reagentPosA;
	VIAL_POS_t reagentPosB;
	VIAL_POS_t reagentPosC;
	VIAL_POS_t reagentPosD;

    enum
    {
        MIX_TABLE_COUNT = 20,
        MIX_TABLE_VAILD_COUNT = MIX_TABLE_COUNT - 1,
    };

	LCAS_MIX_TABLE_t mixTable[MIX_TABLE_COUNT];

	_LCAS_MIX_t()
	{
        Init();
	}

    void Init()
    {
        useMixPrgm = 0;
        relativeDestination = 0;
        relativeReagentA = 0;
        relativeReagentB = 0;
        relativeReagentC = 0;
        relativeReagentD = 0;
    }

    // 참고 사항
    // sizeof(LCAS_MIX_t);	        // 428
    // sizeof(mixTable);            // 400
    // sizeof(LCAS_MIX_TABLE_t);    // 20
    static int SizeExcludeTable()
    {
        return sizeof(_LCAS_MIX_t) - sizeof(mixTable);
    }
} LCAS_MIX_t;


enum States
{
	IdleState = 0, InitializeState, ReadyState, RunState, AnalysisState,
	AdjustState, WashState, DiagnosisState, TempCalibState, PauseState,
	FaultState
};

typedef struct _LCAS_STATE_t
{
	unsigned char state;

	// 현재 실행대기중 이거나 실행중인 샘플 바이알 
	// state가 run이거나 Sequence wait
	VIAL_POS_t sample;
	VIAL_POS_t injectVial;		// 인젝션할 바이알 - mix Program에서 사용
	int curInjCnt;				// 현재 주입 실행 횟수 - 주입이 끝난 시점에서의 횟수(인젝션후 세척동작은 포함 안함).
	float fTemp;				// tray의 현재 온도 
	unsigned char tempReady;	// 온도 안정 여부 - 현재온도와 설정값과의 차가 0.1도 이내로 1분이상 유지 
	unsigned char errorCode;

	_LCAS_STATE_t()
	{
		memset(this, 0, sizeof(struct _LCAS_STATE_t));
	}

} LCAS_STATE_t;


// ad hoc protocols
typedef struct _LCAS_COMMAND_t
{
	enum Commands
	{
		None, Start, Last, Pause, Resume, Abort, Init, Load, Inject, Stop
	};
	unsigned char btCommand;
	unsigned char btSubCommand1;
	unsigned char btSubCommand2;
    unsigned char dummy;

	_LCAS_COMMAND_t(Commands com, byte sub1 = 0, byte sub2 = 0)
	{
		btCommand = (unsigned char)com;
		btSubCommand1 = sub1;
		btSubCommand2 = sub2;
        dummy = 0;
	}
} LCAS_COMMAND_t;


typedef struct _LCAS_SERVICE_t
{
	enum Services
	{
		None, Adjust, Diagnosis, Calibration, Exchange, Wash, UsedTime, MacInit
	};

	unsigned char command;
	unsigned char subCommand1;
	unsigned char subCommand2;
	float value;

	_LCAS_SERVICE_t(Services svr, byte sub1 = 0, byte sub2 = 0, float v = 0.0f)
	{
		command = (unsigned char)svr;
		subCommand1 = sub1;
		subCommand2 = sub2;
		value = v;
	}
} LCAS_SERVICE_t;



typedef struct _LCAS_SEQUENCE_t
{
	VIAL_POS_t firstSamplePos;		// 시작 바이알 위치
	VIAL_POS_t lastSamplePos;			// 끝 바이알 위치
	unsigned char injectionTimes;	// 바이알 당 주입횟수(1~INJECTION_TIMES_MAX)
} LCAS_SEQUENCE_t;


typedef struct _LCAS_SELFMSG_t
{
	// 상태가 변할 때 전송된다.
	enum Messages
	{
		None, State, Error, StartLoading, StartInjection, ExtOut, Stop, RejectCommand, EndSequence, UserResponse, Update
	};

	enum Errors
	{
		NoError, UnknownCommand, NoAdjustData, SkipMissingVial, MissingVial, OperationError, DoorOpened, Leakage, TemperatureError,
		MotorXError, MotorYError, MotorZError, MotorSyrError, MotorValveError, MotorInjError, PositionXYError, PositionZError
	};

	enum StartLoadings
	{
		PCIN, EXIN, AUTO
	};

	enum UserResponses
	{
		NoAction, Skip, Retry, Abort
	};

	enum Updates
	{
		NeedDeviceUpdate, NeedSoftwareUpdate, Success, Failure
	};

	unsigned char ucMessage;
	unsigned char ucNewValue;
    
    char dummy[2];

} LCAS_SELFMSG_t;



typedef struct _LCAS_DIAG_DATA 
{
	unsigned char Current;			// Diagnosis subcommand1

	unsigned char N5Dcheck;			// Analog -5V
	unsigned char V5Dcheck;			// Analog +5V
	unsigned char V1_5Dcheck;		// FPGA Core +1.5V
	unsigned char V12Dcheck;		// Heater Power +12V

	unsigned char V2_5Dcheck;		// ADC Reference Voltage +2.5V
	unsigned char V3_3Dcheck;		// CPU Power +3.3V

	float N5Dvalue;
	float V5Dvalue;
	float V1_5Dvalue;
	float V12Dvalue;
	float V2_5Dvalue;
	float V3_3Dvalue;

	unsigned char Tempcheck;		// 온도	
	unsigned char btHeaterTest;		// 히터(펠티에) 검사, 30도에서 40도까지 상승시간 검사
	unsigned char btCoolerTest;		// 쿨러(페티에) 검사, 40도에서 20도까지 하강시간 검사

	unsigned char btLeakTest1;		// leak check
	unsigned char btLeakTest2;		// leak check

	unsigned char btTempSensor;		// 온도 센서 - 전압(저항)값 검사 및 저항 변화율 검사,
	// 일정시간동안 히팅 후에 저항값 변화 검사

	_LCAS_DIAG_DATA()
	{
		memset(this, 0, sizeof(struct _LCAS_DIAG_DATA));

		N5Dcheck = V5Dcheck = V1_5Dcheck = V12Dcheck = V2_5Dcheck = V3_3Dcheck = -1;
		Tempcheck = btHeaterTest = btCoolerTest = btLeakTest1 = btLeakTest2 = btTempSensor = -1;
	}
} LCAS_DIAG_DATA_t;


typedef struct _LCAS_SPECIAL {
	unsigned char LED_onoff;
	unsigned char Door_check;
	unsigned char Leak_check;
	unsigned char Buzzer_onoff;

	unsigned char Reserved[4];

	_LCAS_SPECIAL()
	{
		memset(this, 0, sizeof(struct _LCAS_SPECIAL));
		Leak_check = 1;
	}

} LCAS_SPECIAL_t;
