//
//
// JBot_C v0.3.0 in JAVA
// Robot for RealTimeBattle v1.0.2
// (c) 1999, 2000 by Ingo Beckmann
// ingo.beckmann@fernuni-hagen.de
// http://www.nnTec.de
//
//
import java.util.*;  
import java.io.*;

// What a Coward !
class JBot_C extends JBot {

    // Hello ! I am:
    final private static String myDefaultName = "Coward";
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
	robotsLeft;
    
    boolean otherRobotEnemy,
	rotationReached;
    
    // Variables:
    double rotate,
	flee;
    
    // Constructor:
    JBot_C (String name, int SLEEP, double timeShare) {
	super(name);
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
	rotationReached = true;
	flee = 0.0;

    } // end Constructor

    // Constructor:
    JBot_C (int SLEEP, double timeShare) {
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
		    //System.err.println(msg2robotString);
		    msg2robot = parseMessage(msg2robotString);
		    
		    // get number of parameters and put them in array of String:
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
			    sendCommand("Shoot 20");
			    sendCommand("Sweep " +(ROT_CANNON+ROT_RADAR)+ " 1 0 0");
			    if (rotationReached && (robotDistance < 9.0)) {
				sendCommand("RotateAmount "+ROT_ROBOT+" 1.5 "+TWO_PI/4);
				rotationReached = false;
				sendCommand("Brake 0");
				sendCommand("Accelerate 1.0");
				flee = time + 1 + 2*Math.random();
				//echoDebug("saw a robot, now fleeing for " +(int)((flee-time)*1000)+ "ms", 4);
			    } else if (robotDistance < 3.0) {
				sendCommand("Brake 0");
				sendCommand("Accelerate -0.5");
			    }
			    break;
			case SHOT:
			    break;
			case WALL:
			    wallDistance = radarDistance;
			    wallAngle = radarAngle;
			    if (wallDistance < 6.0) {
				sendCommand("Brake 1.0");
				sendCommand("Sweep " +(ROT_CANNON+ROT_RADAR)+ " 1 " +(-TWO_PI/8)+ " " +TWO_PI/8);
			    } else {
				sendCommand("Sweep " +(ROT_CANNON+ROT_RADAR)+ " 1.5 " +(-TWO_PI/3)+ " " +TWO_PI/3);
			    }
			    break;
			case COOKIE:
			    sendCommand("Rotate " +ROT_ROBOT+ " 0");
			    sendCommand("Sweep " +(ROT_CANNON+ROT_RADAR)+ " 1 0 0");
			    sendCommand("Brake 0");
			    sendCommand("Accelerate 1.0");
			    break;
			case MINE:
			    if (radarDistance<8.0) sendCommand("Shoot 1");
			    break;
			default:
			    break;
			}
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
			    sendCommand("Sweep " +(ROT_CANNON+ROT_RADAR)+ " 1 0 0");
			    echo(myDefaultName+ ": time to say good bye...");
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
			//echoDebug("hit something at angle=" +collisionAngle, 4);
			switch (collisionObjectType) {
			case ROBOT:
			    sendCommand("Shoot 10");
			    if (rotationReached) {
				sendCommand("RotateAmount "+ROT_ROBOT+" 0.5 " +TWO_PI/3 );
				rotationReached = false;
				sendCommand("Brake 0");
				sendCommand("Accelerate 1");
				flee = time + 1 + 2*Math.random();
				//echoDebug("A robot! Panicing for " +(int)((flee-time)*1000)+ "ms", 4);
				echoDebug("Oh, excuse me! My Name is "+myDefaultName, 4);
			    }
			    break;
			case SHOT:
			    if (rotationReached) {
				sendCommand("RotateAmount "+ROT_ROBOT+" 0.5 " +(-TWO_PI/3) );
				rotationReached = false;
			    }
			    sendCommand("Brake 0");
			    sendCommand("Accelerate 2");
			    flee = time + 1 + 2*Math.random();
			    //echoDebug("A shot! Panicing for " +(int)((flee-time)*1000)+ "ms", 4);
			    echoDebug("Ouch!", 4);
			    break;
			case WALL:
			    if (time > flee) {
				if (rotationReached) {
				    sendCommand("RotateAmount " +ROT_ROBOT+ " 1.5 " +
						((collisionAngle+TWO_PI/2)%TWO_PI) );
				    rotationReached = false;
				}
				sendCommand("Brake 1.0");
				sendCommand("Accelerate 0");
			    }
			    break;
			case COOKIE:
			    echoDebug("Yumm Yumm", 3);
			    break;
			case MINE:
			    echoDebug("I hit a mine!", 3);
			    break;
			default:
			    break;
			}
			s = "Collision Message";
			echoDebug(s, 5);
			break;
			
		    case ENERGY:
			energyLevel = parseDouble(parString[0]);
			// Energy lower than 70% ?
			if (energyLevel*10 < gameOption[ROBOT_START_ENERGY]*7) {
			    handoverBaton = true;
			    sendCommand("Sweep " +(ROT_CANNON+ROT_RADAR)+ " 1 0 0");
			    echo(myDefaultName+ ": low on energy, leaving...");
			}
			echoDebug(s, 5);
			break;
			
		    case ROBOTS_LEFT:
      			robotsLeft = Integer.parseInt(parString[0]);
			if (robotsLeft == 2) {
			    handoverBaton = true;
			    sendCommand("Sweep " +(ROT_CANNON+ROT_RADAR)+ " 1 0 0");
			    echo(myDefaultName+ ": Hey, where are you ?");
			}
			s = "RobotsLeft Message: "+robotsLeft;
			echoDebug(s, 5);
			break;
			
		    case ROTATION_REACHED:
			// Integer.parseInt(parString[0]);
			rotationReached = true;
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
			sendCommand("Rotate " +ROT_ROBOT+ " 1.5");
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
			echo("Bye, bye.");
			break;

		    case YOUR_NAME:
			break;

		    case YOUR_COLOUR:
			break;

		    case UNKNOWN_MESSAGE_TO_ROBOT:
			s = msg2robotString + " with " + numberPars + " parameters";
			echo("*** Unknown Message *** : " + s);
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
		} // bf not ready
		
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
		echoDebug("*** ArrayIndexOutOfBoundsException ***:"+msg2robotString, 1);
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
    
} // end JBot_C
