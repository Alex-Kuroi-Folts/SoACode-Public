#include "stdafx.h"
#include "PlanetRenderStage.h"

#include <Vorb/graphics/GLStates.h>
#include <Vorb/ui/GameWindow.h>

#include "Chunk.h"
#include "ChunkManager.h"
#include "ChunkIOManager.h"
#include "DebugRenderer.h"
#include "GLProgramManager.h"
#include "GameManager.h"
#include "MeshManager.h"
#include "Options.h"
#include "Player.h"
#include "Planet.h"
#include "SkyboxRenderer.h"
#include "Texture2d.h"
#include "VoxelEditor.h"
#include "VoxelWorld.h"

PlanetRenderStage::PlanetRenderStage(const Camera* camera) :
    IRenderStage(camera) {
    // Empty
}

void PlanetRenderStage::draw() {

    f32m4 VP = _camera->getProjectionMatrix() * _camera->getViewMatrix();

    DepthState::FULL.set();
    glBlendFunc(GL_ONE, GL_ZERO);
    GameManager::planet->draw(0, _camera, f32v3(1.0f, 0.0f, 0.0f), 0.1 /*_ambientLight + 0.1*/, _camera->getNearClip() / planetScale, true /*connectedToPlanet*/);

    DepthState::READ.set();
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    RasterizerState::CULL_CLOCKWISE.set();
    if (true /*connectedToPlanet*/) {
        if (!drawMode) GameManager::planet->atmosphere.draw((float)0, VP, glm::vec3((GameManager::planet->invRotationMatrix) * glm::vec4(1.0f, 0.0f, 0.0f, 1.0)), _camera->getPosition());
    } else {
        if (!drawMode) GameManager::planet->atmosphere.draw((float)0, VP, f32v3(1.0f, 0.0f, 0.0f), _camera->getPosition());
    }


    DepthState::FULL.set();
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Chunk::vboIndicesID);
    GameManager::planet->drawTrees(VP, _camera->getPosition(), 0.1f /*ambVal*/);
}