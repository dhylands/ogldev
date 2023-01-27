/*
    Copyright 2022 Etay Meiri

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <cerrno>
#include <string.h>

#include "terrain.h"
#include "texture_config.h"

//#define DEBUG_PRINT

BaseTerrain::~BaseTerrain()
{
    Destroy();
}


void BaseTerrain::Destroy()
{
    m_heightMap.Destroy();
    m_triangleList.Destroy();
}



void BaseTerrain::InitTerrain(float WorldScale, float TextureScale, const std::vector<string>& TextureFilenames)
{
    if (!m_terrainTech.Init()) {
        printf("Error initializing tech\n");
        exit(0);
    }
	
    if (TextureFilenames.size() != ARRAY_SIZE_IN_ELEMENTS(m_pTextures)) {
        printf("%s:%d - number of provided textures (%lld) is not equal to the size of the texture array (%lld)\n",
               __FILE__, __LINE__, TextureFilenames.size(), ARRAY_SIZE_IN_ELEMENTS(m_pTextures));
        exit(0);
    }

    m_worldScale = WorldScale;
    m_textureScale = TextureScale;
    m_isSingleTexTerrain = false;

    for (int i = 0 ; i < ARRAY_SIZE_IN_ELEMENTS(m_pTextures) ; i++) {
        m_pTextures[i] = new Texture(GL_TEXTURE_2D);
        m_pTextures[i]->Load(TextureFilenames[i]);
    }
}


void BaseTerrain::InitTerrain(float WorldScale, float TextureScale)
{
    if (!m_singleTexTerrainTech.Init()) {
        printf("Error initializing tech\n");
        exit(0);
    }

    m_worldScale = WorldScale;
    m_textureScale = TextureScale;
    m_isSingleTexTerrain = true;
}



float BaseTerrain::GetHeightInterpolated(float x, float z) const
{
    float BaseHeight = GetHeight((int)x, (int)z);

    if (((int)x + 1 >= m_terrainSize) ||  ((int)z + 1 >= m_terrainSize)) {
        return BaseHeight;
    }

    float NextXHeight = GetHeight((int)x + 1, (int)z);

    float RatioX = x - floorf(x);

    float InterpolatedHeightX = (float)(NextXHeight - BaseHeight) * RatioX + (float)BaseHeight;

    float NextZHeight = GetHeight((int)x, (int)z + 1);

    float RatioZ = z - floorf(z);

    float InterpolatedHeightZ = (float)(NextZHeight - BaseHeight) * RatioZ + (float)BaseHeight;

    float FinalHeight = (InterpolatedHeightX + InterpolatedHeightZ) / 2.0f;

    return FinalHeight;
}


void BaseTerrain::LoadFromFile(const char* pFilename)
{
    LoadHeightMapFile(pFilename);

    m_triangleList.CreateTriangleList(m_terrainSize, m_terrainSize, this);
}


void BaseTerrain::LoadHeightMapFile(const char* pFilename)
{
    int FileSize = 0;
    unsigned char* p = (unsigned char*)ReadBinaryFile(pFilename, FileSize);

    if (FileSize % sizeof(float) != 0) {
        printf("%s:%d - '%s' does not contain an whole number of floats (size %d)\n", __FILE__, __LINE__, pFilename, FileSize);
        exit(0);
    }

    m_terrainSize = (int)sqrtf((float)FileSize / (float)sizeof(float));

    printf("Terrain size %d\n", m_terrainSize);

    if ((m_terrainSize * m_terrainSize) != (FileSize / sizeof(float))) {
        printf("%s:%d - '%s' does not contain a square height map - size %d\n", __FILE__, __LINE__, pFilename, FileSize);
        exit(0);
    }

    m_heightMap.InitArray2D(m_terrainSize, m_terrainSize, (float*)p);
}


void BaseTerrain::Render(const BasicCamera& Camera)
{
    Matrix4f VP = Camera.GetViewProjMatrix();

    if (m_isSingleTexTerrain) {  
        m_singleTexTerrainTech.Enable();
        m_singleTexTerrainTech.SetVP(VP);
    }
    else {
        m_terrainTech.Enable();
        m_terrainTech.SetVP(VP);
    }

    for (int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(m_pTextures); i++) {
        if (m_pTextures[i]) {
            m_pTextures[i]->Bind(COLOR_TEXTURE_UNIT_0 + i);
        }
    }
	
    m_terrainTech.SetLightDir(m_lightDir);

    m_triangleList.Render();
}


void BaseTerrain::SetMinMaxHeight(float MinHeight, float MaxHeight)
{
    m_minHeight = MinHeight;
    m_maxHeight = MaxHeight;

    if (m_isSingleTexTerrain) {
        m_singleTexTerrainTech.Enable();
        m_singleTexTerrainTech.SetMinMaxHeight(MinHeight, MaxHeight);
    }
    else {
        m_terrainTech.Enable();
        m_terrainTech.SetMinMaxHeight(MinHeight, MaxHeight);
    }
}