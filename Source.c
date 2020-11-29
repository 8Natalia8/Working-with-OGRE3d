#pragma comment(lib, "OgreBites.lib")
#pragma comment(lib, "OgreRTShaderSystem.lib")
#include <exception>
#include <iostream>
#include "Ogre.h"
#include "OgreApplicationContext.h"
#include "OgreRTShaderSystem.h"
#include "OgreCameraMan.h"
#include "OgreInput.h"
using namespace Ogre;
using namespace OgreBites;
class SceneApp : public ApplicationContext, public InputListener
{
public:
    SceneApp();
    virtual void setup(void);
private:
    Ogre::SceneManager* scnMgr;
    Ogre::Root* root;
    void createwater(std::string water);
    void createobj(std::string mesh);
};
SceneApp::SceneApp() : OgreBites::ApplicationContext("OgreScene"){}
void SceneApp::createobj (std::string mesh)
{
    Entity* penguin = scnMgr->createEntity("Mypenguin", mesh);
    penguin->setCastShadows(true);
    SceneNode* penguinNode = scnMgr->getRootSceneNode()->createChildSceneNode();
    penguinNode->setScale(1.8,1.8,1.8);
    penguinNode->setPosition(80.0, 190.0, 180.0);
    penguinNode->attachObject(penguin);
   
    scnMgr->setShadowTechnique(ShadowTechnique::SHADOWTYPE_STENCIL_ADDITIVE);//тень
    //создание камеры
    Ogre::SceneNode* camNode = scnMgr->getRootSceneNode()->createChildSceneNode();
    camNode->setAutoTracking(true, penguinNode);
    camNode->setFixedYawAxis(true);
    Ogre::Camera* cam = scnMgr->createCamera("myCam");
    camNode -> setPosition(100, 300, 400);
    camNode -> lookAt(Vector3(0, 0, 0), Node::TransformSpace::TS_WORLD);
    cam->setNearClipDistance(40);//минимальная дистанция после кот камера не отображает сетку
    cam->setFarClipDistance(50000);
    cam->setAutoAspectRatio(true);
    camNode->attachObject(cam);

    Animation* anim1 = scnMgr->createAnimation("penguinTrack", 4);  
    anim1->setInterpolationMode(Animation::IM_SPLINE);
    NodeAnimationTrack* animtrack1 = anim1->createNodeTrack(0, penguinNode);
    // createNodeKeyFrame-задание времени, когда будет выполняться эта анимация
    animtrack1->createNodeKeyFrame(0.0)->setTranslate(Vector3(0, 20, 0));
    animtrack1->createNodeKeyFrame(2.0)->setTranslate(Vector3(0, 30, 0));
    animtrack1->createNodeKeyFrame(3.0)->setTranslate(Vector3(0, 40, 0));
    animtrack1->createNodeKeyFrame(4.0)->setTranslate(Vector3(0, 50, 0));
    auto animState = scnMgr->createAnimationState("penguinTrack");
    animState->setEnabled(true);
    auto& controllerMgr = ControllerManager::getSingleton();
    controllerMgr.createFrameTimePassthroughController(AnimationStateControllerValue::create(animState, true));
    Animation* anim2 = scnMgr->createAnimation("PenguinTrackRotate", 4);
    anim2->setInterpolationMode(Animation::IM_SPLINE);
    NodeAnimationTrack* animtrack2 = anim2->createNodeTrack(0, penguinNode);
    animtrack2->createNodeKeyFrame(0)->setRotation(Ogre::Quaternion(Ogre::Degree(0), Ogre::Vector3::UNIT_X));
    animtrack2->createNodeKeyFrame(0.5)->setRotation(Ogre::Quaternion(Ogre::Degree(45), Ogre::Vector3::UNIT_X));
    animtrack2->createNodeKeyFrame(1)->setRotation(Ogre::Quaternion(Ogre::Degree(90), Ogre::Vector3::UNIT_X));
    animtrack2->createNodeKeyFrame(1.5)->setRotation(Ogre::Quaternion(Ogre::Degree(135), Ogre::Vector3::UNIT_X));
    animtrack2->createNodeKeyFrame(2)->setRotation(Ogre::Quaternion(Ogre::Degree(180), Ogre::Vector3::UNIT_X));
    animtrack2->createNodeKeyFrame(2.5)->setRotation(Ogre::Quaternion(Ogre::Degree(225), Ogre::Vector3::UNIT_X));
    animtrack2->createNodeKeyFrame(3)->setRotation(Ogre::Quaternion(Ogre::Degree(270), Ogre::Vector3::UNIT_X));
    animtrack2->createNodeKeyFrame(3.5)->setRotation(Ogre::Quaternion(Ogre::Degree(315), Ogre::Vector3::UNIT_X));
    animtrack2->createNodeKeyFrame(4)->setRotation(Ogre::Quaternion(Ogre::Degree(360), Ogre::Vector3::UNIT_X));
    auto animState2 = scnMgr->createAnimationState("PenguinTrackRotate");
    animState2->setEnabled(true);
    auto& controllerMgr2 = ControllerManager::getSingleton();
    controllerMgr2.createFrameTimePassthroughController(AnimationStateControllerValue::create(animState2, true));  
    getRenderWindow()->addViewport(cam);
    return;
}

//создание плоскости
void SceneApp::createwater(std::string water)
{
    Plane plane(Vector3::UNIT_Y, -10);//определяем плоскость
    //создаем плоскость в памяти компьютера
    MeshManager::getSingleton().createPlane(
        "plane", RGN_DEFAULT,
        plane,
        1500, 1500, 200, 200,
        true,
        1, 5, 5,
        Vector3::UNIT_Z);
    //создаем экземпляр плоскости
    Entity* ent= scnMgr->createEntity("LightPlaneEntity","plane");
    //подключаем плоскость к сцене
    scnMgr->getRootSceneNode()->createChildSceneNode()->attachObject(ent);
   ent->setCastShadows(false);//земля не бросает тень
    ent->setMaterialName(water);
    return;
}
void SceneApp::setup(void){
    OgreBites::ApplicationContext::setup();
    root = getRoot();
    scnMgr = root->createSceneManager();
    Ogre::RTShader::ShaderGenerator* shadergen = Ogre::RTShader::ShaderGenerator::getSingletonPtr();
    shadergen->addSceneManager(scnMgr);
  scnMgr->setSkyBox(true, "Examples/CloudySky");
  //прожектор
  Light* spotLight = scnMgr->createLight("SpotLight");
  spotLight->setDiffuseColour(0.93, 0.93, 0.0);
  spotLight->setSpecularColour(1.0, 1.0, 0.0);
  spotLight->setType(Light::LT_SPOTLIGHT);
  SceneNode* spotLightNode = scnMgr->getRootSceneNode()->createChildSceneNode();
  spotLightNode->attachObject(spotLight);
  spotLightNode->setDirection(-1, -1, 0);
  spotLightNode->setPosition(Vector3(630, 600, -40));
  spotLight->setSpotlightRange(Degree(15), Degree(20));
  Light* directionalLight = scnMgr->createLight("directionalLight");
  directionalLight->setType(Ogre::Light::LT_DIRECTIONAL);
  directionalLight->setDiffuseColour(Ogre::ColourValue(0.9, 0.9, 0.9));
  directionalLight->setSpecularColour(Ogre::ColourValue(0.9, 0.9, 0.0));
  //точечный свет
  Light* pointLight = scnMgr->createLight("PointLight");
  pointLight->setType(Light::LT_POINT);
  pointLight->setDiffuseColour(0.0, 0.0, 0.8);
  pointLight->setSpecularColour(0.0, 1.0, 0.8);
  SceneNode* pointLightNode = scnMgr->getRootSceneNode()->createChildSceneNode();
  pointLightNode->attachObject(pointLight);
  pointLightNode->setPosition(Vector3(0, 150, 250));
  createwater("Examples/BeachStones");
  createObject("penguin.mesh");
}
int main(int argc, char* argv[]){
    SceneApp app;
    app.initApp();
    app.getRoot()->startRendering();
    app.closeApp();
    return 0;}
