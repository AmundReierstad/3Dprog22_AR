#include "renderwindow.h"
#include <QTimer>
#include <QMatrix4x4>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLDebugLogger>
#include <QKeyEvent>
#include <QStatusBar>
#include <QDebug>
#include <string>

#include "door.h"
#include "shader.h"
#include "mainwindow.h"
#include "logger.h"
#include "xyz.h"
#include"trianglesurface.h"
#include"cube.h"
//#include"graph.h"  //oblig 2 matte
#include"gridworld.h"
//#include"ground_oblig3.h"
#include"octahedronball.h"
#include"pickup.h"
#include"house.h"
#include"door.h"
#include"heightmapsurface.h"
#include"light.h"

RenderWindow::RenderWindow(const QSurfaceFormat &format, MainWindow *mainWindow)
    : mContext(nullptr), mInitialized(false), mMainWindow(mainWindow)

{
    //This is sent to QWindow:
    setSurfaceType(QWindow::OpenGLSurface);
    setFormat(format);
    //Make the OpenGL context
    mContext = new QOpenGLContext(this);
    //Give the context the wanted OpenGL format (v4.1 Core)
    mContext->setFormat(requestedFormat());
    if (!mContext->create()) {
        delete mContext;
        mContext = nullptr;
        qDebug() << "Context could not be made - quitting this application";
    }

    //Make the gameloop timer:
    mRenderTimer = new QTimer(this);

   //Objects declarations map/hash implementation----------------------------- more effective structure (searching):------------------------------------
    mMap.insert(std::pair<std::string, VisualObject*>{"xyz",new XYZ{}});
    //-----------------Matte oblig 2:
//    mMap.insert(std::pair<std::string, VisualObject*>{"4_4_4",new Graph(true)});
//    mMap.insert(std::pair<std::string, VisualObject*>{"4_4_4",new Graph(false)});
    //-------------------------------
//    mMap.insert(std::pair<std::string, VisualObject*>{"cube",new cube{}});
//    myDisc= new Disc;
//    mMap.insert(std::pair<std::string, VisualObject*>{"disc",myDisc});
//    mMap.insert(std::pair<std::string, VisualObject*>{"triangleSurface",new TriangleSurface("TriangleSurfaceFile.txt")});  //    VisualObject* a=mMap.at("4_4_4");

    //-------------------Oblig 2 prog objects:
    //environment
//       mMap.insert(std::pair<std::string, VisualObject*>{"ground", new ground_oblig3()});
       mMap.insert(std::pair<std::string, VisualObject*>{"grid",new GridWorld(3)});
    //6-8 trophies
       mMap.insert(std::pair<std::string, VisualObject*>{"pickup0",new Pickup(4,0.2f)});
       mMap.insert(std::pair<std::string, VisualObject*>{"pickup1",new Pickup(4,0.2f)});
       mMap.insert(std::pair<std::string, VisualObject*>{"pickup2",new Pickup(4,0.2f)});
       mMap.insert(std::pair<std::string, VisualObject*>{"pickup3",new Pickup(4,0.2f)});
       mMap.insert(std::pair<std::string, VisualObject*>{"pickup4",new Pickup(4,0.2f)});
       mMap.insert(std::pair<std::string, VisualObject*>{"pickup5",new Pickup(4,0.2f)});
    //npc
     mMap.insert(std::pair<std::string, VisualObject*>{"npc",new cube()});
    //player char
     mMap.insert(std::pair<std::string, VisualObject*>{"player", new OctahedronBall(4,0.2f)});

     //house, with slot for door
     mMap.insert(std::pair<std::string, VisualObject*>{"house", new House()});
    //door
     mMap.insert(std::pair<std::string, VisualObject*>{"door", new Door()});

     //oblig 3 prog objects:
     //textureTest
     mMap.insert(std::pair<std::string, VisualObject*>{"triangle",new TriangleSurface()});
     mMap.insert(std::pair<std::string, VisualObject*>{"tri",new TriangleSurface()});
     mMap.insert(std::pair<std::string, VisualObject*>{"heightMap",new HeightMapSurface("iceland_heightmap.png",0.1)});
     mMap.insert(std::pair<std::string, VisualObject*>{"light",new light()});



     //moving static objects/ pickups;
     mMap.at("pickup0")->move(1,0.3,2);
     mMap.at("pickup1")->move(1,0.3,-2);
     mMap.at("pickup2")->move(2,0.3,3);
     mMap.at("pickup3")->move(0,0.3,3);
     mMap.at("pickup4")->move(-1,0.3,0);
     mMap.at("pickup5")->move(6,0.3,0);
     mMap.at("house")->move(4,0,-2);
     mMap.at("house")->mMatrix.rotate(-90,{0,1,0});
     mMap.at("door")->move(4,0,0);
     mMap["player"]->move(0.0f,0.2f,0.0f);

     mMap.at("tri")->mMatrix.setColumn(3,{1,1,1,1});
}

RenderWindow::~RenderWindow()
{
    //cleans up the GPU memory
    glDeleteVertexArrays( 1, &mVAO );
    glDeleteBuffers( 1, &mVBO );
}

// Sets up the general OpenGL stuff and the buffers needed to render a triangle
void RenderWindow::init()
{
    //Get the instance of the utility Output logger
    //Have to do this, else program will crash (or you have to put in nullptr tests...)
    mLogger = Logger::getInstance();

    //Connect the gameloop timer to the render function:
    //This makes our render loop
    connect(mRenderTimer, SIGNAL(timeout()), this, SLOT(render()));
    //********************** General OpenGL stuff **********************

    //The OpenGL context has to be set.
    //The context belongs to the instanse of this class!
    if (!mContext->makeCurrent(this)) {
        mLogger->logText("makeCurrent() failed", LogType::REALERROR);
        return;
    }
    //just to make sure we don't init several times
    //used in exposeEvent()
    if (!mInitialized)
        mInitialized = true;
    //must call this to use OpenGL functions
    initializeOpenGLFunctions();
    //Print render version info (what GPU is used):
    //Nice to see if you use the Intel GPU or the dedicated GPU on your laptop
    // - can be deleted
    mLogger->logText("The active GPU and API:", LogType::HIGHLIGHT);
    std::string tempString;
    tempString += std::string("  Vendor: ") + std::string((char*)glGetString(GL_VENDOR)) + "\n" +
            std::string("  Renderer: ") + std::string((char*)glGetString(GL_RENDERER)) + "\n" +
            std::string("  Version: ") + std::string((char*)glGetString(GL_VERSION));
    mLogger->logText(tempString);
    //Start the Qt OpenGL debugger
    //Really helpfull when doing OpenGL
    //Supported on most Windows machines - at least with NVidia GPUs
    //reverts to plain glGetError() on Mac and other unsupported PCs
    // - can be deleted
    startOpenGLDebugger();
    //general OpenGL stuff:
    glEnable(GL_DEPTH_TEST);            //enables depth sorting - must then use GL_DEPTH_BUFFER_BIT in glClear
    //    glEnable(GL_CULL_FACE);       //draws only front side of models - usually what you want - test it out!
    glClearColor(0.4f, 0.4f, 0.4f, 1.0f);    //gray color used in glClear GL_COLOR_BUFFER_BIT

    //Compile shaders:
    setUpShader();
    setUpPhongShader();

    //-------------------------------------------------Camera, Objects-----------------------
    mCamera.init(mPmatrixUniform,mVmatrixUniform,mPmatrixUniformPhong,mVmatrixUniformPhong);

    for (auto it=mMap.begin(); it!=mMap.end(); it++)
    {
        if (it->first=="door" || it->first=="tri" )
        {
        (*it).second->init(mMatrixUniformPhong);
        }
        else
        (*it).second->init(mMatrixUniform);
    }
    glBindVertexArray(0);       //unbinds any VertexArray - good practice
}

void RenderWindow::setUpShader()
{
    //NB: hardcoded path to files! You have to change this if you change directories for the project.
    //Qt makes a build-folder besides the project folder. That is why we go down one directory
    // (out of the build-folder) and then up into the project folder.
    mShaderProgram = new Shader("../3Dprog22_AR/plainshader.vert", "../3Dprog22_AR/plainshader.frag");
    // Get the matrixUniform location from the shader
    // This has to match the "matrix" variable name in the vertex shader
    // The uniform is used in the render() function to send the model matrix to the shader
    mMatrixUniform = glGetUniformLocation( mShaderProgram->getProgram(), "matrix" );
    mPmatrixUniform = glGetUniformLocation( mShaderProgram->getProgram(), "pMatrix" );                              //can setupshader within own function and call here, cleaner look
    mVmatrixUniform = glGetUniformLocation( mShaderProgram->getProgram(), "vMatrix" );

}

void RenderWindow::setUpPhongShader()
{

    mShaderProgramPhong = new Shader("../3Dprog22_AR/PhongVertexShader.vert", "../3Dprog22_AR/PhongFragmentShader.frag");

    mMatrixUniformPhong = glGetUniformLocation( mShaderProgramPhong->getProgram(), "mMatrix" );
    mPmatrixUniformPhong = glGetUniformLocation( mShaderProgramPhong->getProgram(), "pMatrix" );
    mVmatrixUniformPhong = glGetUniformLocation( mShaderProgramPhong->getProgram(), "vMatrix" );

}

// Called each frame - doing the rendering!!!
void RenderWindow::render()
{
    //casting:
    OctahedronBall* player=dynamic_cast<OctahedronBall*>(mMap.at("player"));
    Door* door=dynamic_cast<Door*>(mMap.at("door"));
    HeightMapSurface* heightMap=dynamic_cast<HeightMapSurface*>(mMap.at("heightMap"));
    light* lightPhong=dynamic_cast<light*>(mMap.at("light"));
    mTimeStart.restart(); //restart FPS clock
    mContext->makeCurrent(this); //must be called every frame (every time mContext->swapBuffers is called)

    initializeOpenGLFunctions();    //must call this every frame it seems...

    //clear the screen for each redraw
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //what shader to use
    glUseProgram(mShaderProgram->getProgram() );

//------Camera update------------------------  :
    heightMap->CheckPlayerHeight(*player);
    mCamera.init(mPmatrixUniform,mVmatrixUniform,mPmatrixUniformPhong,mVmatrixUniformPhong);
    mCamera.perspective(60, 4.0/3.0, 0.1, 10.0);


    //check if in house
   if(mMap.at("player")->mMatrix.column(3).z()>=-2
      && mMap.at("player")->mMatrix.column(3).z()<=2
      && mMap.at("player")->mMatrix.column(3).x()>=4
      && mMap.at("player")->mMatrix.column(3).x()<=8)
   {
       //camera in house
       mCamera.lookAt(mMap.at("house")->mMatrix.column(3).toVector3D()+QVector3D{0,3,0} ,mMap.at("player")->mMatrix.column(3).toVector3D(),mCamera.mUpDirection);
   }
   else
   {
       //camera outside house
       mCamera.lookAt(mCamera.mPosition ,mMap.at("player")->mMatrix.column(3).toVector3D(),mCamera.mUpDirection);
   }
//  mCamera.update();

//---------Move objects------------------  :
   mMap.at("npc")->move(2);
   mMap.at("light")->move(1);

    //myDisc->move(10);
    //    mMap["disc"]->move(0.017f);

//----------Collision check-----------------------------:

      bool collision{false};
      for(int i{};i<6;i++)
      {
          std::string stringHash{"pickup"};
          stringHash+=std::to_string(i);
          Pickup* collisionObject=dynamic_cast<Pickup*>(mMap.at(stringHash));
          if (!collisionObject->hasCollided)
          {
               collision=player->checkCollision(*collisionObject);
               if(collision)
               {
                  collisionObject->onCollision();
                  mLogger->logText("AliBabar", LogType::HIGHLIGHT);

               }

          }

      }

    door->onCollision(door->checkCollision(*player));

//_______INDEV_____

//     mLogger->logText("Player Location xyz: "+ std::to_string(player->mMatrix.column(3).x())+std::to_string(player->mMatrix.column(3).y())+std::to_string(player->mMatrix.column(3).x())+std::to_string(player->mMatrix.column(3).z()), LogType::HIGHLIGHT);


    //------------Draw------------ :
    //draw objects in map
    for (auto it=mMap.begin(); it!=mMap.end(); it++)
    {
        if (it->first=="door" || it->first=="tri")
        {
        glUseProgram(mShaderProgramPhong->getProgram() );
        mShaderProgramPhong->setVec3("lightPosition",lightPhong->curPos);
         mCamera.update("phong");
         (*it).second->draw();
//          mLogger->logText("PhongDongr", LogType::HIGHLIGHT);
        }
        else
        {
         glUseProgram(mShaderProgram->getProgram() );
          mCamera.update("plain");
        (*it).second->draw();
        }
    }
//    for (auto it=mObjects.begin(); it != mObjects.end(); it++)
//    {
//        (*it)->draw();
//    }

    calculateFramerate();
    checkForGLerrors(); //using our expanded OpenGL debugger to check if everything is OK.
    mContext->swapBuffers(this);

//------------Rotation------------- :
    //simple rotation around z axis
//    if(mRotate)
//    {
//        for (auto it=mMap.begin(); it!=mMap.end(); it++)
//        {
//            (*it).second->mMatrix.rotate(2.f, 0.f, 1.0, 0.f);
//        }
//    }


            // mMatrix->rotate(2.f, 0.f, 1.0, 0.f);
}

//This function is called from Qt when window is exposed (shown)
// and when it is resized
//exposeEvent is a overridden function from QWindow that we inherit from
void RenderWindow::exposeEvent(QExposeEvent *)
{
    //if not already initialized - run init() function - happens on program start up
    if (!mInitialized)
        init();

    //This is just to support modern screens with "double" pixels (Macs and some 4k Windows laptops)
    const qreal retinaScale = devicePixelRatio();

    //Set viewport width and height to the size of the QWindow we have set up for OpenGL
    glViewport(0, 0, static_cast<GLint>(width() * retinaScale), static_cast<GLint>(height() * retinaScale));

    //If the window actually is exposed to the screen we start the main loop
    //isExposed() is a function in QWindow
    if (isExposed())
    {
        //This timer runs the actual MainLoop
        //16 means 16ms = 60 Frames pr second (should be 16.6666666 to be exact...)
        mRenderTimer->start(16);
        mTimeStart.start();
    }
}

//The way this function is set up is that we start the clock before doing the draw call,
// and check the time right after it is finished (done in the render function)
//This will approximate what framerate we COULD have.
//The actual frame rate on your monitor is limited by the vsync and is probably 60Hz
void RenderWindow::calculateFramerate()
{
    long nsecElapsed = mTimeStart.nsecsElapsed();
    static int frameCount{0};                       //counting actual frames for a quick "timer" for the statusbar

    if (mMainWindow)            //if no mainWindow, something is really wrong...
    {
        ++frameCount;
        if (frameCount > 30)    //once pr 30 frames = update the message == twice pr second (on a 60Hz monitor)
        {
            //showing some statistics in status bar
            mMainWindow->statusBar()->showMessage(" Time pr FrameDraw: " +
                                                  QString::number(nsecElapsed/1000000.f, 'g', 4) + " ms  |  " +
                                                  "FPS (approximated): " + QString::number(1E9 / nsecElapsed, 'g', 7));
            frameCount = 0;     //reset to show a new message in 30 frames
        }
    }
}

//Uses QOpenGLDebugLogger if this is present
//Reverts to glGetError() if not
void RenderWindow::checkForGLerrors()
{
    if(mOpenGLDebugLogger)  //if our machine got this class to work
    {
        const QList<QOpenGLDebugMessage> messages = mOpenGLDebugLogger->loggedMessages();
        for (const QOpenGLDebugMessage &message : messages)
        {
            if (!(message.type() == message.OtherType)) // get rid of uninteresting "object ...
                                                        // will use VIDEO memory as the source for
                                                        // buffer object operations"
                // valid error message:
                mLogger->logText(message.message().toStdString(), LogType::REALERROR);
        }
    }
    else
    {
        GLenum err = GL_NO_ERROR;
        while((err = glGetError()) != GL_NO_ERROR)
        {
            mLogger->logText("glGetError returns " + std::to_string(err), LogType::REALERROR);
            switch (err) {
            case 1280:
                mLogger->logText("GL_INVALID_ENUM - Given when an enumeration parameter is not a "
                                "legal enumeration for that function.");
                break;
            case 1281:
                mLogger->logText("GL_INVALID_VALUE - Given when a value parameter is not a legal "
                                "value for that function.");
                break;
            case 1282:
                mLogger->logText("GL_INVALID_OPERATION - Given when the set of state for a command "
                                "is not legal for the parameters given to that command. "
                                "It is also given for commands where combinations of parameters "
                                "define what the legal parameters are.");
                break;
            }
        }
    }
}

//Tries to start the extended OpenGL debugger that comes with Qt
//Usually works on Windows machines, but not on Mac...
void RenderWindow::startOpenGLDebugger()
{
    QOpenGLContext * temp = this->context();
    if (temp)
    {
        QSurfaceFormat format = temp->format();
        if (! format.testOption(QSurfaceFormat::DebugContext))
            mLogger->logText("This system can not use QOpenGLDebugLogger, so we revert to glGetError()",
                             LogType::HIGHLIGHT);

        if(temp->hasExtension(QByteArrayLiteral("GL_KHR_debug")))
        {
            mLogger->logText("This system can log extended OpenGL errors", LogType::HIGHLIGHT);
            mOpenGLDebugLogger = new QOpenGLDebugLogger(this);
            if (mOpenGLDebugLogger->initialize()) // initializes in the current context
            mLogger->logText("Started Qt OpenGL debug logger");
        }
    }
}

//Event sent from Qt when program receives a keyPress
// NB - see renderwindow.h for signatures on keyRelease and mouse input
// Button press interacticity:
void RenderWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape)
    {
      mMainWindow->close();       //Shuts down the whole program
    }

    //You get the keyboard input like this
   //Player
   if(event->key() == Qt::Key_A)
    {
       mMap["player"]->move(-0.1f,0.0f,0.0f);
    }
   if(event->key() == Qt::Key_D)
    {
       mMap["player"]->move(0.1f,0.0f,0.0f);

    }
   if(event->key() == Qt::Key_W)
    {
       mMap["player"]->move(0.0f,0.0f,-0.1f);
    }
   if(event->key() == Qt::Key_S)
    {
       mMap["player"]->move(0.0f,0.0f,0.1f);
    }
   //camera movementIJKL
   QVector3D crossCamera=crossCamera.normal(mCamera.mFrontDirection,mCamera.mUpDirection);
   if(event->key() == Qt::Key_J)
   {
      mCamera.mPosition-=crossCamera*mCamera.cameraSpeed;
   }
   if(event->key() == Qt::Key_L)
   {
    mCamera.mPosition+=crossCamera*mCamera.cameraSpeed;
   }
   if(event->key() == Qt::Key_I)
   {
       mCamera.mPosition+=mCamera.cameraSpeed*mCamera.mFrontDirection;
   }
   if(event->key() == Qt::Key_K)
   {
       mCamera.mPosition-=mCamera.cameraSpeed*mCamera.mFrontDirection;
   }
   if(event->key() == Qt::Key_E)
   {
//    Pickup* collisionObject=dynamic_cast<Pickup*>(mMap.at(stringHash));
       cube* npc=dynamic_cast<cube*>(mMap.at("npc"));
       if( npc->doParabel==true)
          npc->doParabel=false;
       else npc->doParabel=true;
   }


//    if(event->key() == Qt::Key_S)
//    {
//        mMainWindow->statusBar()->showMessage(" SSSS");
//    }
}
