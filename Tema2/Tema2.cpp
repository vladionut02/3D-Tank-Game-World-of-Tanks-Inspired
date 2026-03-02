#include "lab_m1/Tema2/Tema2.h"

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <random>



using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */



Tema2::Tema2()
{
}


Tema2::~Tema2()
{
}









void Tema2::Init()
{
    renderCameraTarget = false;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> randomSize(10, 20);
    std::uniform_int_distribution<> randomPosition(0, 100);
    camera = new implemented::Camera();
    camera->Set(glm::vec3(0, 2, 3.5f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));


    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;

        mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;

        mesh = new Mesh("corp");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "corp.obj");
        meshes[mesh->GetMeshID()] = mesh;

        mesh = new Mesh("turela");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "turela.obj");
        meshes[mesh->GetMeshID()] = mesh;

        mesh = new Mesh("tun");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "tun.obj");
        meshes[mesh->GetMeshID()] = mesh;

        mesh = new Mesh("senile");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "senile.obj");
        meshes[mesh->GetMeshID()] = mesh;

        mesh = new Mesh("plane");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "plane50.obj");
        meshes[mesh->GetMeshID()] = mesh;


        MyTank tank{};


        for (int i = 0; i < nrCladiri; i++) {
            validPosition = false;
            xBuildingPosition = randomPosition(gen);
            zBuildingPosition = randomPosition(gen);
            buildingPosition = glm::vec3(xBuildingPosition - 50, 7, zBuildingPosition - 50);
            distance = glm::distance(buildingPosition, tank.position);
            if (distance > 25) { // 15.0f is half of the building's size
                validPosition = true;
            }
            if (validPosition) {

                glm::mat4 modelMatrix = glm::mat4(1.0f);
                modelMatrix = glm::translate(modelMatrix, buildingPosition);
                modelMatrix = glm::scale(modelMatrix, glm::vec3(randomSize(gen), randomSize(gen), randomSize(gen)));
                cladiri.push_back(modelMatrix);
            }
            else {
                i--;
            }



        }



    }



    // Create a shader program for drawing face polygon with the color of the normal
    {
        Shader* shader = new Shader("LabShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }
}


Mesh* Tema2::CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned int>& indices)
{
    unsigned int VAO = 0;
    // Create the VAO and bind it
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Create the VBO and bind it
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Send vertices data into the VBO buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

    // Create the IBO and bind it
    unsigned int IBO;
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

    // Send indices data into the IBO buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

    // ========================================================================
    // This section demonstrates how the GPU vertex shader program
    // receives data.

    // TODO(student): If you look closely in the `Init()` and `Update()`
    // functions, you will see that we have three objects which we load
    // and use in three different ways:
    // - LoadMesh   + LabShader (this lab's shader)
    // - CreateMesh + VertexNormal (this shader is already implemented)
    // - CreateMesh + LabShader (this lab's shader)
    // To get an idea about how they're different from one another, do the
    // following experiments. What happens if you switch the color pipe and
    // normal pipe in this function (but not in the shader)? Now, what happens
    // if you do the same thing in the shader (but not in this function)?
    // Finally, what happens if you do the same thing in both places? Why?

    // Set vertex position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);

    // Set vertex normal attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));

    // Set texture coordinate attribute
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3)));

    // Set vertex color attribute
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));
    // ========================================================================

    // Unbind the VAO
    glBindVertexArray(0);

    // Check for OpenGL errors
    CheckOpenGLError();

    // Mesh information is saved into a Mesh object
    meshes[name] = new Mesh(name);
    meshes[name]->InitFromBuffer(VAO, static_cast<unsigned int>(indices.size()));
    meshes[name]->vertices = vertices;
    meshes[name]->indices = indices;
    return meshes[name];

}


void Tema2::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema2::Update(float deltaTimeSeconds)
{
    {

        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(4, 0, 4));
        RenderSimpleMesh(meshes["plane"], shaders["LabShader"], modelMatrix, glm::vec3(0, 0.9, 1));

        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, tank.position);
        modelMatrix = glm::rotate(modelMatrix, tank.rotation, glm::vec3(0, 1, 0));
        RenderSimpleMesh(meshes["corp"], shaders["LabShader"], modelMatrix, glm::vec3(0.02f, 0.3f, 0.12f));
        RenderSimpleMesh(meshes["senile"], shaders["LabShader"], modelMatrix, glm::vec3(0, 0, 0));

        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, tank.position);
        modelMatrix = glm::rotate(modelMatrix, tank.rotation, glm::vec3(0, 1, 0));
        modelMatrix = glm::rotate(modelMatrix, tank.turelaRot, glm::vec3(0, 1, 0));
        RenderSimpleMesh(meshes["turela"], shaders["LabShader"], modelMatrix, glm::vec3(0.08f, 0.46f, 0.2f));
        RenderSimpleMesh(meshes["tun"], shaders["LabShader"], modelMatrix, glm::vec3(0, 0, 0));



    }


    {
        // Render buildings
        for (const auto& clad : cladiri) {
            RenderSimpleMesh(meshes["box"], shaders["LabShader"], clad, glm::vec3(0.2f, 1, 0.5f));
        }
        // Render projectile
        for (const auto& projectile : projectiles) {
            glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), projectile.position);
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.6f));
            RenderSimpleMesh(meshes["sphere"], shaders["LabShader"], modelMatrix, glm::vec3(0.0f, 0.0f, 0));
        }

    }

    {
        for (auto& projectile : projectiles) {
            projectile.Update(deltaTimeSeconds);

        }
    }
}


void Tema2::FrameEnd()
{
    //DrawCoordinateSystem();
}


void Tema2::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, glm::vec3& color)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Use the specified shader
    glUseProgram(shader->program);

    // Set model matrix
    glUniformMatrix4fv(glGetUniformLocation(shader->program, "Model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Set view matrix
    glm::mat4 viewMatrix = CalculateViewMatrix();
    glUniformMatrix4fv(glGetUniformLocation(shader->program, "View"), 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Set projection matrix
    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    glUniformMatrix4fv(glGetUniformLocation(shader->program, "Projection"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    // Set color
    glUniform3f(glGetUniformLocation(shader->program, "Color"), color.x, color.y, color.z);

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);

    mesh->Render();
}

glm::mat4 Tema2::CalculateViewMatrix()
{
    glm::vec3 cameraPosition = CalculateCameraPosition();
    glm::vec3 cameraTarget = CalculateCameraTarget(cameraPosition);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    return glm::lookAt(cameraPosition, cameraTarget, cameraUp);
}

glm::vec3 Tema2::CalculateCameraPosition()
{
    tankPosition = tank.position;
    tankRotation = tank.rotation;
    cameraRotation = tankRotation + cameraRot - AI_MATH_PI / 2;

    rotationQuat = glm::angleAxis(cameraRotation, glm::vec3(0.0f, 1.0f, 0.0f));
    rotatedOffset = glm::rotate(rotationQuat, cameraOffset);

    return tankPosition + rotatedOffset;
}

glm::vec3 Tema2::CalculateCameraTarget(const glm::vec3& cameraPosition)
{
    tankPosition = tank.position;
    tankRotation = tank.rotation;
    cameraRotation = tankRotation + cameraRot - AI_MATH_PI / 2;

    rotationQuat = glm::angleAxis(cameraRotation, glm::vec3(0.0f, 1.0f, 0.0f));
    rotatedOffset = glm::rotate(rotationQuat, cameraOffset);
    lookAtDirection = glm::vec3(sin(cameraRotation), 2.5f, cos(cameraRotation));
    cameraTarget = tankPosition + lookAtDirection;

    cameraTarget -= 0.5f * rotatedOffset;
    return cameraTarget;
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */



void Tema2::OnInputUpdate(float deltaTime, int mods)
{
    // Add key press event
    tankAngle = tank.rotation - AI_MATH_PI / 2;
    tankSpeed = 5;

    tankForward = glm::normalize(glm::vec3(sin(tankAngle), 0, cos(tankAngle)));

    // Check the state of keys and update the tank's position accordingly
    if (window->KeyHold(GLFW_KEY_W))
    {
        currentPosition = tank.position + tankForward * tankSpeed * deltaTime;
        tank.position = currentPosition;
    }
    if (window->KeyHold(GLFW_KEY_S))
    {
        currentPosition = tank.position - tankForward * tankSpeed * deltaTime;
        tank.position = currentPosition;
    }
    if (window->KeyHold(GLFW_KEY_A))
    {
        currentRotation = tank.rotation + deltaTime;
        tank.rotation = currentRotation;
    }
    if (window->KeyHold(GLFW_KEY_D))
    {
        currentRotation = tank.rotation - deltaTime;
        tank.rotation = currentRotation;
    }

}


void Tema2::OnKeyPress(int key, int mods)
{

}


void Tema2::OnKeyRelease(int key, int mods)
{
    // Add key release event

}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event


    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        float sensitivity = 0.001f;
        if (rightMousePressed)
        {
            cameraRot -= deltaX * sensitivity;
        }
        else
        {
            rightMousePressed = true;
        }
    }
    else
    {

        float turretRotationSpeed = 0.005f;


        float currentRotation = tank.turelaRot - deltaX * turretRotationSpeed;
        tank.turelaRot = currentRotation;

        rightMousePressed = false;
    }
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
    if (button == 1) {

        turretRotation = tank.turelaRot + tank.rotation - AI_MATH_PI / 2;
        // Calculate x and z components of the direction
        turretDirection = glm::vec3(glm::sin(turretRotation), 0.0f, glm::cos(turretRotation));

        // Calculate the position of the projectile in front of the cannon
        cannonFront = 6.0f * glm::normalize(turretDirection);
        cannonFront.y += 1.75f;
        cannonFront.z += -0.2f;
        initialPosition = tank.position + cannonFront;

        // Calculate velocity based on turret direction
        velocity = glm::normalize(turretDirection) * projectileSpeed; // Adjust projectileSpeed as needed

        // Create a new projectile at the calculated position
        projectiles.emplace_back(initialPosition, velocity);
    }
}



void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event

}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema2::OnWindowResize(int width, int height)
{
}
