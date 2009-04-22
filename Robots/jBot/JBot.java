//
//
// JBot v0.2.0 in JAVA
// Robot for RealTimeBattle v1.0.5
// 'include'-file & some methods
// by Ingo Beckmann
// ingo.beckmann@fernuni-hagen.de
// http://www.nnTec.de
//
//
class JBot extends Thread {

    // Robot Name:
    String Name;
    // Robot & Game flags:
    static boolean dead = false, 
       gameFinishes = false,
       exitRobot = false,
       handoverBaton = false;
    // Debug Level: 1=Exceptions, 2=Warnings, 3=rare events, 
    //              4=frequent events, 5=trivial messages
    private double debugLevel;

    // Some Mathematics:
    public static final double TWO_PI = 2.0*Math.PI;
    // and, most important (in milliseconds):
    public static final int DEFAULT_SLEEP = 4;
    // default timeshare [s], should be big enough for a robot fighting 'til end of game:
    public static final double UNTIL_END = 10000.0;

        // Constants:
    public static final int
       MAX_PARAMETER_COUNT = 4,
       WARNING_TYPE_COUNT = 7,
       GAME_OPTION_COUNT = 15;

    // make GameOptions accessible for 'other' robots...
    static double[] gameOption = new double[GAME_OPTION_COUNT];

    // Warning Types:
    public static final int
       UNKNOWN_MESSAGE = 0,
       PROCESS_TIME_LOW = 1,
       //ENERGY_LOW,
       //VARIABLE_OUT_OF_RANGE,
       MESSAGE_SENT_IN_ILLEGAL_STATE = 2,
       UNKNOWN_OPTION = 3,
       OBSOLETE_KEYWORD = 4,
       NAME_NOT_GIVEN = 5,
       COLOUR_NOT_GIVEN = 6;

    // Game Option Types:
    public static final int
       ROBOT_MAX_ROTATE = 0,
       ROBOT_CANNON_MAX_ROTATE = 1,
       ROBOT_RADAR_MAX_ROTATE = 2,

       ROBOT_MAX_ACCELERATION = 3,
       ROBOT_MIN_ACCELERATION = 4,

       ROBOT_START_ENERGY = 5,
       ROBOT_MAX_ENERGY = 6,
       ROBOT_ENERGY_LEVELS = 7,

       SHOT_SPEED = 8,
       SHOT_MIN_ENERGY = 9,
       SHOT_MAX_ENERGY = 10,
       SHOT_ENERGY_INCREASE_SPEED = 11,

       TIMEOUT = 12,
       DEBUG_LEVEL = 13,             // 0 - no debug, 5 - highest debug level
       SEND_ROBOT_COORDINATES= 14;    // 0 - no coordinates, 
                                     // 1 - coordinates are given relative the starting position
                                     // 2 - absolute coordinates        
    // Robot Option Types:
    public static final int 
       SIGNAL = 2,                   // 0 - no signal, > 1 - signal to send (e.g. SIGUSR1 or SIGUSR2) 
       SEND_SIGNAL = 0,              // 0 - false, 1 - true
       SEND_ROTATION_REACHED = 1,    // 0 - no messages
                                     // 1 - messages when RotateTo and RotateAmount finished
                                     // 2 - messages also when sweep direction is changed

       USE_NON_BLOCKING = 3;         // 0 - false, 1 - true 
                                     // This option should always be sent as soon as possible
       
    // Object Types:
    public static final int 
	NOOBJECT = -1, 
	ROBOT = 0, 
	SHOT = 1, 
	WALL = 2, 
	COOKIE = 3, 
	MINE = 4,
	LAST_OBJECT_TYPE = 5;

    // Rotation Object Types:
    public static final int 
	ROT_ROBOT = 1, 
	ROT_CANNON = 2, 
	ROT_RADAR = 4; 

    // Messages to Robot:
    public static final int 
	INITIALIZE = 1001,
	YOUR_NAME = 1002,
	YOUR_COLOUR = 1003,
	GAME_OPTION = 1004,
	GAME_STARTS = 1005,
	RADAR = 1006,
	INFO = 1007,
	ROBOT_INFO = 1008,
	ROTATION_REACHED = 1009,
	ENERGY = 1010,
	ROBOTS_LEFT = 1011,
	COLLISION = 1012,
	WARNING = 1013,
	DEAD = 1014,
	GAME_FINISHES = 1015,
	EXIT_ROBOT = 1016,
	// this line was added by Johannes Nicolai
	COORDINATES=1017,
	UNKNOWN_MESSAGE_TO_ROBOT = -1;


    // JBot Constructor:
    JBot (String Name) {
	super();
	this.Name = Name;

	if (dead || gameFinishes || handoverBaton) {
	    debugLevel = gameOption[DEBUG_LEVEL]; // with predecessor take old debug level
	} else {
	    debugLevel = 3;    // so I see the GameOptions in the beginning
	}

	dead = false;
	gameFinishes = false;
	exitRobot = false;
	handoverBaton = false;
    }

    public final int parseMessage(String s) {
	if (s == null) return UNKNOWN_MESSAGE_TO_ROBOT;
	if (s.equals("Radar")) return RADAR;
	if (s.equals("Info")) return INFO;
	if (s.equals("RobotInfo")) return ROBOT_INFO;
	if (s.equals("Collision")) return COLLISION;
	if (s.equals("Energy")) return ENERGY;
	if (s.equals("RotationReached")) return ROTATION_REACHED;
	if (s.equals("RobotsLeft")) return ROBOTS_LEFT;
	if (s.equals("GameOption")) return GAME_OPTION;
	if (s.equals("GameStarts")) return GAME_STARTS;
	if (s.equals("Initialize")) return INITIALIZE;
	if (s.equals("Warning")) return WARNING;
	if (s.equals("Dead"))  return DEAD;
	if (s.equals("GameFinishes"))  return GAME_FINISHES;
	if (s.equals("ExitRobot"))  return EXIT_ROBOT;
	if (s.equals("YourName"))  return YOUR_NAME;
	if (s.equals("YourColour"))  return YOUR_COLOUR;
	// this line was added by Johannes Nicolai 
	if (s.equals("Coordinates"))  return COORDINATES;
	return UNKNOWN_MESSAGE_TO_ROBOT;
    }


    // Here are some common methods for all JBots:
    public final double parseDouble (String s) {
	return Double.valueOf(s).doubleValue();
    }
	    
    public final void echo (String s) {
	sendCommand("Print " + s);
    }

    public final void echoDebug (String s, double dl) {
	// !!! MAX_DEBUG_LEVEL = 5 !!!
	// for a competitive robot, you might want to comment out the following line:
	if ( (debugLevel>=dl) && (dl>0.0) && (dl<=5.0) ) sendCommand("Debug " + s);
    }

    public final void sendCommand (String s) {
	System.out.println( s );
	System.out.flush();
    }
    
    final void setDebugLevel (double dl) {
       debugLevel = dl;
    }

}
