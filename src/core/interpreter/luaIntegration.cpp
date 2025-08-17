#include "luaIntegration.h"

#include "../entity.h"
#include "../gameObject.h"
#include "../scene.h"
#include "../sceneManager.h"
#include "../camera2d.h"
#include "../../maths/maths.h"

using namespace maths;

namespace core
{
    using namespace ui;
    
    namespace lua
    {
        /**
         * Faz chamadas ao log do sistema.
         */
        void kx_log_(const char* msg, LogType type = LogType::INFO)
        {
            LogWindow::Log(msg, type);
        }
        
        /**
         * Integração de métodos
         */
        void in_functions_(sol::state& lua)
        {
            // Utilização de overload para argumento padrão
            lua.set_function("log", 
                sol::overload(
                    [](const char* msg, LogType type){ kx_log_(msg, type); },
                    [](const char* msg){ kx_log_(msg); }
                )
            );
        }
        
        /**
         * Integração de enumeradores
         */
        void in_enumerators_(sol::state& lua)
        {
            // Sistematica de logs
            lua.new_enum("LogType",
                "Info", LogType::INFO,
                "Warn", LogType::WARNING,
                "Error", LogType::ERROR
            );
        }
        
        /**
         * Integração de classes
         */
        void in_classes_(sol::state& lua)
        {
            // Entity
            lua.new_usertype<Entity>("Entity",
                sol::constructors<Entity(const char*, Transform2D)>(),
                                     
                // Props
                "Transform", &Entity::transform,
                
                // Methods
                "setPosition", &Entity::setLocalPosition,
                "setScale", &Entity::setLocalScale,
                "setAngle", &Entity::setLocalAngle,
                
                "setName", &Entity::setName,
                "getName", &Entity::getName,
                
                "getID", &Entity::GetID
            );
            
            // GameObject
            lua.new_usertype<GameObject>("GameObject",
                sol::constructors<GameObject(const char*)>(),
                sol::base_classes, sol::bases<Entity>(),
                                         
                // Props
                "Color", &GameObject::color,
                "Shape", &GameObject::shape
            );
            
            // Camera
            lua.new_usertype<Camera2D>("Camera",
                sol::constructors<Camera2D(const char*, const SceneManager&)>(),
                "Projection", &Camera2D::projection,
                "View", &Camera2D::view,

                "WorldMouse", &Camera2D::worldMouse,
                "Position", &Camera2D::position
            );
            
            // SceneManager (Não é permitida a construção de SceneManager)
            lua.new_usertype<SceneManager>("SceneManager",
                "getActiveScene", &SceneManager::GetActiveScene,
                "getActiveCamera", &SceneManager::GetActiveCamera
            );
            
            // Scenes (Podem ser construídas utilizando o manager padrão)
            lua.new_usertype<Scene>("Scene",
                sol::constructors<Scene(SceneManager&)>(),
                "camera", &Scene::mainCamera,
                
                // Gerenciamento de Objetos
                "addObject", &Scene::AddObject,
                "addGameObject", &Scene::AddGameObject,

                "getObjectByName", &Scene::GetObject,
                "getObjectList", &Scene::GetObjectList,

                "delObjectWithName", &Scene::RemoveObject,
                "clearScene", &Scene::ClearScene
            );
            
            /**
             * Matematica
             */
            
            // Vectors
            
            // Vector2
            lua.new_usertype<vec2>("vec2",
                sol::constructors<
                    vec2(),
                    vec2(float, float)
                >(),
                "x", &vec2::x,
                "y", &vec2::y,
                
                sol::meta_function::addition, &vec2::operator+,
                sol::meta_function::subtraction, &vec2::operator-,
                
                sol::meta_function::division, sol::overload(
                    static_cast<vec2(vec2::*)(float) const>(&vec2::operator/),
                    static_cast<vec2(vec2::*)(const vec2&) const>(&vec2::operator/)
                ),
                sol::meta_function::multiplication, sol::overload(
                    static_cast<vec2(vec2::*)(float) const>(&vec2::operator*),
                    static_cast<vec2(vec2::*)(const vec2&) const>(&vec2::operator*)
                )
            );
            
            // Vector 3
            lua.new_usertype<vec3>("vec3",
                sol::constructors<
                    vec3(), 
                    vec3(float, float, float)
                >(),
                sol::base_classes, sol::bases<vec2>(),
                "z", &vec3::z,
                
                sol::meta_function::addition, &vec3::operator+,
                sol::meta_function::subtraction, &vec3::operator-,
                
                sol::meta_function::division, sol::overload(
                    static_cast<vec3(vec3::*)(float) const>(&vec3::operator/),
                    static_cast<vec3(vec3::*)(const vec3&) const>(&vec3::operator/)
                ),
                sol::meta_function::multiplication, sol::overload(
                    static_cast<vec3(vec3::*)(float) const>(&vec3::operator*),
                    static_cast<vec3(vec3::*)(const vec3&) const>(&vec3::operator*)
                )
            );
            
            // Vector 4
            lua.new_usertype<vec4>("vec4",
                sol::constructors<
                    vec4(), 
                    vec4(float, float, float, float)
                >(),
                sol::base_classes, sol::bases<vec3>(),
                "w", &vec4::w,
                
                sol::meta_function::addition, &vec4::operator+,
                sol::meta_function::subtraction, &vec4::operator-,
                
                sol::meta_function::division, sol::overload(
                    static_cast<vec4(vec4::*)(float) const>(&vec4::operator/),
                    static_cast<vec4(vec4::*)(const vec4&) const>(&vec4::operator/)
                ),
                sol::meta_function::multiplication, sol::overload(
                    static_cast<vec4(vec4::*)(float) const>(&vec4::operator*),
                    static_cast<vec4(vec4::*)(const vec4&) const>(&vec4::operator*)
                )
            );
        }
    }
}
