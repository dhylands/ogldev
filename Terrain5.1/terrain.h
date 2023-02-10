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


#ifndef TERRAIN_H
#define TERRAIN_H

#include "ogldev_types.h"
#include "ogldev_basic_glfw_camera.h"
#include "ogldev_array_2d.h"
#include "ogldev_texture.h"
#include "triangle_list.h"
#include "terrain_technique.h"
#include "slope_scale_lighter.h"

class BaseTerrain
{
 public:
    BaseTerrain() : m_slopeScaleLighter(&m_heightMap) {}

    ~BaseTerrain();

    void Destroy();

	void InitTerrain(float WorldScale, float TextureScale, const std::vector<string>& TextureFilenames, float LightSoftness);

    void Render(const BasicCamera& Camera);

    void LoadFromFile(const char* pFilename);

    void SaveToFile(const char* pFilename);

	float GetHeight(int x, int z) const { return m_heightMap.Get(x, z); }
	
    float GetHeightInterpolated(float x, float z) const;

	float GetWorldScale() const { return m_worldScale; }

    float GetTextureScale() const { return m_textureScale; }

    int GetSize() const { return m_terrainSize; }

    void SetTexture(Texture* pTexture) { m_pTextures[0] = pTexture; }
	
    void SetTextureHeights(float Tex0Height, float Tex1Height, float Tex2Height, float Tex3Height);
	
    Vector3f GetColor(int x, int z) const;

    void SetLightDir(const Vector3f& LightDir);

 protected:

	void LoadHeightMapFile(const char* pFilename);

    void SetMinMaxHeight(float MinHeight, float MaxHeight);

    int m_terrainSize = 0;
	float m_worldScale = 1.0f;
    Array2D<float> m_heightMap;
    TriangleList m_triangleList;
    Texture* m_pTextures[4] = { 0 };
    float m_textureScale = 1.0f;

private:
    Vector3f GetSimpleLighting(int x, int z) const;

    Vector3f GetSlopeScaleLighting(int x, int z) const;

    float m_minHeight = 0.0f;
    float m_maxHeight = 0.0f;
    TerrainTechnique m_terrainTech;
    SlopeScaleLighter m_slopeScaleLighter;
    Vector3f m_lightDir = Vector3f(0.0f, 0.0f, 0.0f);
    float m_lightSoftness = 0.0f;
};

#endif