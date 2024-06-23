//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//
#pragma once
#include <gui/gl/View.h>
#include <gui/Key.h>
#include <gui/Texture.h>
#include <gui/FileDialog.h>
#include "Program.h"
#include <gui/gl/CommandList.h>
#include <gui/gl/Buffer.h>
#include <gui/gl/Textures.h>

//Demonstrates:
// - all in one textures
// - animation 3D mode with Buffer, CommandList, Command, Program,...

class ViewGLAnimation3DTexture2 : public gui::gl::View
{
    using Base = gui::gl::View;
    glm::mat4 _perspectiveMatrix;
    glm::mat4 _viewMatrix;
    glm::mat4 _mvpMatrix;
    
//    GLuint _textureIDs[2];  //two textures
    //gui::gl::Textures _textures;
    gui::gl::Buffer _gpuBuffer;
    
    Program _program;
    gui::gl::Command* pMtxSetterCmd = nullptr;
    gui::gl::Command* pCubeTextureCmd = nullptr;
    
    float originx=0.0, originy=0.0;
    float _angleX = 0;
    float _angleY = 0;
    float _dAngle = 0.01f;
    bool left = false, right = false, radup=false, raddown=false;
    float radius = 16.0f;
    float drad = 0.2f;
    td::UINT4 nVertices = 0;
    std::vector<float> vec;
    std::vector<int>repeat;
    std::vector<bool>lastv;
    float EPS = 0.06;
    int iteracije = 0;
    int granica = 50;

    
//    const unsigned int nTriangles = 6 * 2;   // Number of triangles rendered
    td::BYTE _usedTexture = 0; //0 or 1
private:

    void addPoints() {
        td::String origin = getMessage1();
        std::stringstream aa(origin.c_str());
        char cc;
        aa >> cc >> originx >> cc >> originy;
        iteracije++;
        //_gpuBuffer.remove(pCubeTextureCmd);
        _gpuBuffer.reset();
        _gpuBuffer.init(64, 100, 100, { gui::gl::DataType::vec3});
        pMtxSetterCmd = _gpuBuffer.createCommand();
        pMtxSetterCmd->createMVPSetter(&_mvpMatrix);
        _gpuBuffer.encode(pMtxSetterCmd);

        td::String tackepom = getMessage();
        //if (tackepom == "") return;
        std::string tacke = tackepom.c_str();
        std::stringstream ss(tacke);

        float x, y, z;
        char c;
        
        while (1) {
            ss >> c;
            while (c != 'x') {

                ss >> c;
                if (ss.eof()) break;
            }
            ss >> x;
            if (ss.eof()) break;
            ss >> c;
            while (c != 'y') {
                ss >> c;
                if (ss.eof()) break;
            }
            ss >> y;
            if (ss.eof()) break;
            ss >> c;
            while (c != 'z') {
                ss >> c;
                if (ss.eof()) break;
            }
            ss >> z;
            if (ss.eof()) break;
            x = x + originx;
            y = y + originy;
            bool found = false;
            for (int i = 0; i < vec.size(); i += 3) {
                if (abs(x - vec[i]) <= EPS && abs(y - vec[i + 1]) <= EPS && abs(z - vec[i + 2]) <= EPS) {
                    repeat[i/3]++;
                    lastv[i / 3] = true;
                    found = true;
                    break;
                }
            }
            if(!found) {
                vec.emplace_back(x);
                vec.emplace_back(y);
                vec.emplace_back(z);
                repeat.push_back(0);
                lastv.push_back(true);
            }
            while (vec.size() % 3 != 0) {
                vec.emplace_back(0);
            }
        }
        for (int i = 0; i < repeat.size(); i++) {
            if (repeat[i] >= granica) continue;
            if (!lastv[i]) repeat[i] = 0;
            lastv[i] = false;
        }
        float* niz = new float[vec.size()];
        int ind = 0;
        for (int i = 0; i < vec.size(); i+=3) {
            if (repeat[i/3] >= granica) {
                niz[ind]=vec[i];
                niz[ind + 1] = vec[i + 1];
                niz[ind + 2] = vec[i + 2];
                ind += 3;
            }
        }
        
        nVertices = ind/3;

        _gpuBuffer.appendVertices(niz, nVertices);

        delete[] niz;
        
        pCubeTextureCmd = _gpuBuffer.createCommand();
        pCubeTextureCmd->createDrawArrays(gui::gl::Primitive::Points, 0, nVertices);
        _gpuBuffer.encode(pCubeTextureCmd);
        if (!_gpuBuffer.commit())
        {
            mu::dbgLog("ERROR! Cannot commit buffer to GPU");
            return;
        }

        _program.setBuffer(&_gpuBuffer);

    }
    bool setupShaders()
    {
        if (!_program.link(":shVert", ":shFrag"))
            return false;
        return true;
    }
    
    //setup data and send it to GPU, create command list
    void setup()
    {
        //encode command to set transformation matrix (uniform)
        pMtxSetterCmd = _gpuBuffer.createCommand();
        pMtxSetterCmd->createMVPSetter(&_mvpMatrix);
        _gpuBuffer.encode(pMtxSetterCmd);
        
        // Define the cube's vertices and texture coordinates
        float vertices[] = {
            // Front face (number 1)
            0.5f,   0.5f,  0.5f,    // Vertex 0 (top-right)
           -0.5f,   0.5f,  0.5f,         // Vertex 1 (top-left)
           -0.5f,  -0.5f,  0.5f,          // Vertex 2 (bottom-left)
            0.5f,  -0.5f,  0.5f,      // Vertex 3 (bottom-right)

            // Right facec (number 2)
            0.5f, -0.5f,  0.5f,  // Vertex 8 (bottom-left)
            0.5f, -0.5f, -0.5f, // Vertex 9 (bottom-right)
            0.5f,  0.5f, -0.5f,  // Vertex 10 (top-left)
            0.5f,  0.5f, 0.5f, // Vertex 11 (top-right)
            
            // Back face (number 6)
            0.5f, -0.5f, -0.5f,   // Vertex 0 (top-right)
           -0.5f, -0.5f, -0.5f,  // Vertex 1 (top-left)
           -0.5f,  0.5f, -0.5f,  // Vertex 2 (bottom-left)
            0.5f,  0.5f, -0.5f, // Vertex 3 (bottom-right)
            
            // Left face (number 5)
           -0.5f, -0.5f, -0.5f,   // Vertex 12 (bottom-left)
           -0.5f,  -0.5f, 0.5f,   // Vertex 13 (bottom-right)
           -0.5f,  0.5f,  0.5f,   // Vertex 14 (top-left)
           -0.5f,  0.5f, -0.5f,   // Vertex 15 (top-right)

            // Top face (number 3)
           -0.5f,  0.5f,  0.5f,   // Vertex 16 (bottom-left)
            0.5f,  0.5f,  0.5f,   // Vertex 17 (bottom-right)
            0.5f,  0.5f, -0.5f,   // Vertex 18 (top-left)
           -0.5f,  0.5f, -0.5f,   // Vertex 19 (top-right)

            // Bottom face (number 4)
           -0.5f, -0.5f, -0.5f,    // Vertex 20 (bottom-left)
            0.5f, -0.5f, -0.5f,          // Vertex 21 (bottom-right)
            0.5f, -0.5f,  0.5f,           // Vertex 22 (top-left)
           -0.5f, -0.5f,  0.5f     // Vertex 23 (top-right)
        };
        
        td::UINT4 indices[] =
        {
            0, 1, 2, // Front face
            2, 3, 0, // Front face

            4, 5, 6, // Rigt face
            4, 6, 7, // Rigt face

            8, 9, 11, // Back face
            9, 10, 11, // Back face

            12, 13, 14, // Left face
            12, 14, 15, // Left face

            16, 17, 18, // Top face
            16, 18, 19, // Top face

            20, 21, 22, // Bottom face
            20, 22, 23  // Bottom face
        };

        /*std::fstream f("C:/Users/DiV/Desktop/meh_projekat/tacke.txt", std::ios_base::in);


        float x, y, z;
        float verts2[26421*3];
        int i = 0;
        while (i< 26421) {
            f >> x >> y >> z;
            verts2[3 * i] = x;
            verts2[3 * i+1] = y;
            verts2[3 * i+2] = z;
            i++;
        }
        

        td::UINT4 nVertices = 26421;
        
        _gpuBuffer.appendVertices(verts2, nVertices);
        
        td::UINT4 nIndices = 3*2*6;
        _gpuBuffer.appendIndices(indices, nIndices);*/
        
        //encode command to draw tetured cube
        //pCubeTextureCmd = _gpuBuffer.createCommand();
        //pCubeTextureCmd->createDrawElements(gui::gl::Primitive::Points, 0, nIndices);
        //pCubeTextureCmd->createDrawArrays(gui::gl::Primitive::Points, 0, nVertices);
        //pCubeTextureCmd->setTexture(gui::gl::Textures::Unit::T0, _usedTexture);
        //_gpuBuffer.encode(pCubeTextureCmd);
        
        /*if (!_gpuBuffer.commit())
        {
            mu::dbgLog("ERROR! Cannot commit buffer to GPU");
            return;
        }
        
        // Load textures

        _program.setBuffer(&_gpuBuffer);*/
    }
protected:
    
    void onInit() override
    {
        auto [major, minor] = getOpenGLVersion();
        mu::dbgLog("Used opengl %d.%d", major, minor);
        
        if (!setupShaders())
        {
            mu::dbgLog("ERROR! OpenGL cannnot setup shaders!");
            assert(false);
            return;
        }
        setup();
        
        
        // Set up the perspective parameters
        float fov = 45.0f; // Field of view in degrees
        float aspectRatio = 16.0f / 9.0f; // Aspect ratio of the viewport
        float nearClip = 0.1f; // Near clipping plane
        float farClip = 100.0f; // Far clipping plane

        // Create a perspective matrix
        _perspectiveMatrix = glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip);
        
        // Camera parameters
        glm::vec3 cameraPosition = glm::vec3(10.0, 5.0, 6.0f);  // New camera position
        glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.5f);    // Camera target (where the camera is looking)
        glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);         // Up vector

        // Create a new view matrix
        _viewMatrix = glm::lookAt(cameraPosition, cameraTarget, cameraUp);
        
        _mvpMatrix = _perspectiveMatrix * _viewMatrix; //* I for model
        
        gui::gl::Context::enable(gui::gl::Context::Flag::DepthTest);
        //gui::gl::Context::enable(gui::gl::Context::Flag::CullFace);
        
#ifdef DEBUG_GL
        GLenum error = glGetError();
        if (error != GL_NO_ERROR)
        {
            mu::dbgLog("OpenGL error: %x", error);
        }
#endif
    }
    
    bool prepareNextFrame() override
    {
        addPoints();
        if(left) _angleX += _dAngle;
        if(right) _angleX -= _dAngle;
        if (radup) radius += drad;
        if (raddown && radius > drad) radius -= drad;

        glm::vec3 cameraPosition = glm::vec3(radius*glm::cos(_angleX), 5.0, radius * glm::sin(_angleX));  // New camera position
        glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);    // Camera target (where the camera is looking)
        glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);         // Up vector

        // Create a new view matrix
        _viewMatrix = glm::lookAt(cameraPosition, cameraTarget, cameraUp);


//        _angleY += _dAngle/2;
        // Create a model matrix with rotations around X and Y axes
        glm::mat4 modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::rotate(modelMatrix, -3.1415f/2, glm::vec3(1.0f, 0.0f, 0.0f)); // Rotate around X axis
        //modelMatrix = glm::rotate(modelMatrix, _angleY, glm::vec3(0.0f, 1.0f, 0.0f)); // Rotate around Y axis

        // Combine the perspective, view, and model matrices to get the final MVP matrix
        _mvpMatrix = _perspectiveMatrix * _viewMatrix * modelMatrix;
        return true;
//        reDraw();
    }
    
    void onDraw(const gui::Rect& rect) override
    {
        // Clear
        gui::gl::Context::clear(td::ColorID::Black);
        gui::gl::Context::clear({gui::gl::Context::Clear::Color, gui::gl::Context::Clear::Depth});

        _program.activate();
        
        _program.execute();

        // Unbind program
        _program.deActivate();
    }

    bool onKeyPressed(const gui::Key& key) override {
        if (key.getChar() == 'a' || key.getChar() == 'A') {
            left = true;
        }
        if (key.getChar() == 'd' || key.getChar() == 'D') {
            right = true;
        }
        if (key.getChar() == 'w' || key.getChar() == 'W') {
            raddown = true;
        }
        if (key.getChar() == 's' || key.getChar() == 'S') {
            radup = true;
        }
        return true;
    }
    bool onKeyReleased(const gui::Key& key) override {
        if (key.getChar() == 'a' || key.getChar() == 'A') {
            left = false;
        }
        if (key.getChar() == 'd' || key.getChar() == 'D') {
            right = false;
        }
        if (key.getChar() == 'w' || key.getChar() == 'W') {
            raddown = false;
        }
        if (key.getChar() == 's' || key.getChar() == 'S') {
            radup = false;
        }
        return true;
    }

public:
    ViewGLAnimation3DTexture2()
    {
        //specify buffer layouts before creatin the context
        _gpuBuffer.init(64, 100, 100, {gui::gl::DataType::vec3});
        
        gui::gl::Context reqContext(gui::gl::Context::Animation::Yes, gui::gl::DepthBuffer::Size::B2);
        createContext(reqContext, {gui::InputDevice::Event::Keyboard, gui::InputDevice::Event::PrimaryClicks, gui::InputDevice::Event::SecondaryClicks });
    }
    
    ~ViewGLAnimation3DTexture2()
    {
        makeCurrentContext();
    }
    
    void updateSpeed(float val)
    {
        _dAngle = val;
    }
    
    void switchTexture()
    {
        if (_usedTexture == 0)
            _usedTexture = 1;
        else
            _usedTexture = 0;
        
        pCubeTextureCmd->setTexture(gui::gl::Textures::Unit::T0, _usedTexture);
    }
    
    void exportFrame()
    {

        
       
    }
};
