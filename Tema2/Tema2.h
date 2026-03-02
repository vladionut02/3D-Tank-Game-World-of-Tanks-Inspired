#pragma once

#include <vector>

#include "components/simple_scene.h"
#include "lab_m1/lab5/lab_camera.h" 


namespace m1
{

    struct Projectile {
        glm::vec3 position;
        glm::vec3 velocity;

        Projectile(const glm::vec3& initialPosition, const glm::vec3& initialVelocity)
            : position(initialPosition), velocity(initialVelocity) {}

        void Update(float deltaTimeSeconds) {
            // Update logic based on velocity and deltaTime
            position += velocity * deltaTimeSeconds;
        }
    };
    struct MyTank {
        glm::vec3 position;
        float rotation;
        glm::mat4 modelMatrix;
        float turelaRot;
        MyTank() :
            position(glm::vec3(0, 1, 0)), rotation(0), modelMatrix(glm::mat4(1)), turelaRot(0) {}
    };





    class Tema2 : public gfxc::SimpleScene
    {
    public:
        Tema2();
        ~Tema2();

        void Init() override;
        Mesh* CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned int>& indices);


    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, glm::vec3& color);
        glm::mat4 CalculateViewMatrix();
        glm::vec3 CalculateCameraPosition();
        glm::vec3 CalculateCameraTarget(const glm::vec3& cameraPosition);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;
    protected:
        implemented::Camera* camera;
        glm::mat4 projectionMatrix;
        bool renderCameraTarget;
        MyTank tank;
        std::vector<glm::mat4> cladiri;
        std::vector<Projectile> projectiles;
        bool rightMousePressed = false;
        float cameraRot = 0;
        glm::vec3 currentPosition;
        float currentRotation;
        int nrCladiri = 15;
        float xBuildingPosition;
        float zBuildingPosition;
        glm::vec3 buildingPosition;
        bool validPosition;
        float distance;
        glm::vec3 cameraOffset = glm::vec3(0.0f, 5.0f, -9.0f);
        glm::vec3 tankPosition;
        float tankRotation;
        float cameraRotation;
        glm::quat rotationQuat;
        glm::vec3 rotatedOffset;
        glm::vec3 cameraTarget;
        glm::vec3 lookAtDirection;
        glm::vec3 turretDirection;
        glm::vec3 tankForward;
        float turretRotation;
        float tankAngle;
        float tankSpeed;
        float projectileSpeed = 100;
        glm::vec3 cannonFront;
        glm::vec3 initialPosition;
        glm::vec3 velocity;
    };
}   // namespace m1
