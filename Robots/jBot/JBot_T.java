//
//
// JBot_T v0.2.0 in JAVA
// Robot for RealTimeBattle v0.9.11
// (c) 1999 by Ingo Beckmann
// ingo.beckmann@fernuni-hagen.de
// http://www.nnTec.de
//
//
import java.util.*;  
import java.io.*;

// My sweet little Terminator:
class JBot_T extends JBot {

    // Hello ! I am:
    final private static String myDefaultName = "T100";
    // borussiamoenchengladbachlikecolour:
    final private static String HOMECOLOUR="45c126", AWAYCOLOUR="b3e5d3";
    // sleep in ms, everytime in do-loop:
    private int SLEEP;
    // How much time have I ?
    private double myTimeShare;
    
    private String[] warning;
    
    private boolean continueThis; 
    private StringTokenizer st;
    private BufferedReader bf;
    private String msg2robotString, s;
    private int msg2robot,
	numberPars,
	lastAction,
	lastObject;
    
    private String[] parString;    // Parameters
    
    // Info from RTB:
    double radarDistance,
	radarAngle,
	robotDistance, 
	wallDistance,
	wallAngle,
	collisionAngle,
	time,
	speed,
	energyLevel,
	cannonAngle,
	otherRobotEnergy;
    
    int radarObjectType,
	collisionObjectType,
	optionType,
	warningType,
	robotsLeft,
	rotationReached;
    
    boolean otherRobotEnemy; 
    
    // Variables:
    int salve, maxSalve;
    double rotate;
    boolean rotFlag;
    
    // Constructor:
    JBot_T (String Name, int SLEEP, double timeShare) {
	super(Name);
	this.SLEEP = SLEEP;
	myTimeShare = timeShare;
	
	warning = new String[WARNING_TYPE_COUNT];
	parString = new String[MAX_PARAMETER_COUNT];
	
	bf = new BufferedReader(new InputStreamReader(System.in));
	
	// some Initialization:
	lastAction = UNKNOWN_MESSAGE_TO_ROBOT;
	lastObject = NOOBJECT;
	time = 0.0;
	rotate = 3.0;
	rotFlag = false;
	salve = 0;
	maxSalve = 2;
	robotDistance = 0.0;
	
    } // end Constructor

    // Constructor:
    JBot_T (int SLEEP, double timeShare) {
	// call constructor above with default name:
	this(myDefaultName, SLEEP, timeShare);
    } // end Constructor
    

    // JBot running as a thread:
    public void run () {

	// OK, start operation...
	do {
	    try {
		
		if (bf.ready()) {
		    // There is a message !    
		    // Get the message waiting:
		    st = new StringTokenizer(bf.readLine());
		    
		    // get the message command (first token):
		    if (st.hasMoreTokens()) msg2robotString = st.nextToken();
		    msg2robot = parseMessage(msg2robotString);
		    
		    // get number of parameters and put them in an array of String:
		    numberPars = st.countTokens();
		    for (int i=0; i<numberPars; i++) {
			parString[i] = st.nextToken();
		    }
		    
		    // Okay, let's see what we've received:
		    switch (msg2robot) {
			
		    case RADAR:
			radarDistance = parseDouble(parString[0]);
			radarObjectType = Integer.parseInt(parString[1]);
			radarAngle = parseDouble(parString[2]);
			switch (radarObjectType) {
			case ROBOT:
			    robotDistance = radarDistance;
			    echoDebug("distance to robot: "+robotDistance, 4);
			    sendCommand("Shoot 10");
			    if (robotDistance > 4.0) {
				sendCommand("Rotate " +ROT_ROBOT+ " " +rotate/3);
				sendCommand("Brake 0");
				sendCommand("Accelerate 1.2");
			    } else {
				sendCommand("Rotate " +ROT_ROBOT+ " 0");
				sendCommand("Brake 0");
				sendCommand("Accelerate -0.3");
				//sendCommand("Brake 1");
			    }
			    sendCommand("Shoot 10");
			    if (rotFlag && robotDistance>5.0) {
				rotFlag = false;
				rotate = -rotate;
				echoDebug("reversing my rotation", 4);
			    }
			    break;
			case SHOT:
			    if (salve < maxSalve) {
				sendCommand("Shoot 5");
				sendCommand("Rotate " +ROT_ROBOT+ " " +rotate/10);
				salve++;
				echoDebug("shooting a shot", 5);
				//if (robotDistance>2) sendCommand("RotateAmount "+ROT_ROBOT+" "+rotate/3+" "+1/robotDistance/4);
			    } else {
				sendCommand("Rotate " +ROT_ROBOT+ " 0");
			    }
			    break;
			case WALL:
			    wallDistance = radarDistance;
			    wallAngle = radarAngle;
			    if (wallDistance < 2.0) {
				sendCommand("Brake 1");
			    } else {
				sendCommand("Brake 0");
				sendCommand("Accelerate 1");
			    }
			    rotFlag = true;
			    sendCommand("Rotate " +ROT_ROBOT+ " " +rotate);
			    break;
			case COOKIE:
			    sendCommand("Rotate " +ROT_ROBOT+ " 0");
			    if (speed<4.0) {
				sendCommand("Accelerate 2.0");
			    } else {
				sendCommand("Accelerate 0.0");
				echoDebug("running at maximum-cookie-speed!", 4);
			    }
			    break;
			case MINE:
			    if (radarDistance<8.0) sendCommand("Shoot 1");
			    break;
			default:
			    break;
			}
			if (radarObjectType != SHOT) salve = 0;
			s = "Radar Message";
			echoDebug(s, 5);
			break; // end CASE RADAR
			
		    case INFO:
   			time = parseDouble(parString[0]);
   			speed = parseDouble(parString[1]);
   			cannonAngle = parseDouble(parString[2]);
			// game over for me ?
			if (time > myTimeShare) {
			    handoverBaton = true;
			    echo(myDefaultName+ ": handing over to my successor...");
			}
			s = "Info Message, time: " +time;
			echoDebug(s, 5);
			break;
			
		    case ROBOT_INFO:
			otherRobotEnergy = parseDouble(parString[0]);
			otherRobotEnemy = (Integer.parseInt(parString[1])==0) ? true : false;
			s = "RobotInfo Message";
			echoDebug(s, 5);
			break;
			
		    case COLLISION:
      			collisionObjectType = Integer.parseInt(parString[0]);
			collisionAngle = parseDouble(parString[1]);
			switch (collisionObjectType) {
			case ROBOT:
			    sendCommand("RotateAmount " +ROT_ROBOT+ " " +collisionAngle);
			    break;
			case SHOT:
			    echoDebug("Ouch !", 4);
			    break;
			case WALL:
			    break;
			case COOKIE:
			    echoDebug("Yumm Yumm !", 3);;
			    break;
			case MINE:
			    echoDebug("I hit a mine !", 3);
			    break;
			default:
			    break;
			}
			s = "Collision Message";
			echoDebug(s, 5);
			break;
			
		    case ENERGY:
			energyLevel = parseDouble(parString[0]);
			s = "Energy Message";
			echoDebug(s, 5);
			break;
			
		    case ROBOTS_LEFT:
      			robotsLeft = Integer.parseInt(parString[0]);
			s = "RobotsLeft Message";
			echoDebug(s, 5);
			break;
			
		    case ROTATION_REACHED:
			rotationReached = Integer.parseInt(parString[0]);
			s = "RotationReached Message";
			echoDebug(s, 5);
			break;
			
		    case GAME_OPTION:
			optionType = Integer.parseInt(parString[0]);
			gameOption[optionType] = parseDouble(parString[1]);
			switch (optionType) {
			case DEBUG_LEVEL:
			    setDebugLevel(gameOption[optionType]);
			    break;
			default:
			    break;
			}
			echoDebug("received GameOption " +optionType+ ": "+gameOption[optionType], 3);
			break;

		    case GAME_STARTS:
			echo("Let's go !");
			break;

		    case INITIALIZE:
			if (Integer.parseInt(parString[0])==1) {
			    sendCommand("Name " +Name);
			    sendCommand("Colour " +HOMECOLOUR+ " " +AWAYCOLOUR);
			}
			echo("Alive & Kicking !!!");
			break;

		    case WARNING:
			warningType = Integer.parseInt(parString[0]);
			warning[warningType] = parString[1];
			if (warningType == PROCESS_TIME_LOW) echo("low on CPU-time: " +warning[warningType]);
			echoDebug("Warning Message: " +warningType+ " " + warning[warningType], 2);
			break;
			
		    case DEAD:
			// be quiet from now on:
			dead = true;
			break;

		    case GAME_FINISHES:
			gameFinishes = true;
			break;

		    case EXIT_ROBOT:
			exitRobot = true;
			echo("I'll be back.");
			break;

		    case YOUR_NAME:
			break;

		    case YOUR_COLOUR:
			break;

		    // the coordinate treatment was added by Johannes Nicolai
		    // we do not need any coordinate information
		    case COORDINATES:
			break;

		    case UNKNOWN_MESSAGE_TO_ROBOT:
			s = msg2robotString + " with " + numberPars + " parameters";
			echo("*** Unknown Message ***: " + s);
			break;

		    default:
			break;
		    }  // end switch (msg2robot)
		    
		    // when there is no message:
		} else {      
		    // No new message:
		    echoDebug("is sad. No message for him.", 5);
		    // sleep in my spare time:
		    try {
			int extra_sleep = 2*SLEEP+1;
			echoDebug("gets some extra sleep: " +extra_sleep+ "ms", 5);
			sleep(extra_sleep);
		    } catch (InterruptedException ie) {
			echoDebug("*** InterruptedException ***", 1);
		    }
		}
		
		// do other stuff here
		echoDebug("could do something else", 5);
		
	    } // end try {...}

	    // Exception caused by BufferedReader ?
	    catch (IOException ioX) {  
		echoDebug("*** IOException ***", 1);
	    }
	    // Exception caused by StringTokenizer ?
	    catch (NoSuchElementException nseX) {  
		echoDebug("*** NoSuchElementException ***", 1);
	    }
	    // Exception caused by parsing routines ?
	    catch (NumberFormatException nfeX) {
		echoDebug("*** NumberFormatException ***", 1);
	    }	    
	    catch (ArrayIndexOutOfBoundsException aiobX) {
		echoDebug("*** ArrayIndexOutOfBoundsException ***", 1);
	    }	    
	    catch (Exception x) {
		echoDebug("*** Exception *** "+x.toString(), 1);
	    }	   
	    
	    continueThis = !(dead || gameFinishes || exitRobot || handoverBaton);
	    // Finally find some sleep (guaranteed!):
	    if (continueThis && SLEEP > 0) try {
		echoDebug("sleeping for " +SLEEP+ "ms, ...zzzz...zzz....", 5);
		sleep(SLEEP);
	    } catch (InterruptedException ie) {
		echoDebug("*** InterruptedException ***", 1);
	    }
	    
	} while (continueThis);
    } // end run
    
} // end JBot_T
