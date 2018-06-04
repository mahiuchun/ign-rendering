/*
 * Copyright (C) 2018 Open Source Robotics Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
*/

#include <gtest/gtest.h>
#include <string>

#include <ignition/common/Console.hh>

#include "test_config.h"  // NOLINT(build/include)

#include "ignition/rendering/Visual.hh"
#include "ignition/rendering/RenderEngine.hh"
#include "ignition/rendering/RenderingIface.hh"
#include "ignition/rendering/Scene.hh"

using namespace ignition;
using namespace rendering;

class VisualTest : public testing::Test,
                   public testing::WithParamInterface<const char *>
{
  /// \brief Test visual material
  public: void Material(const std::string &_renderEngine);
};

/////////////////////////////////////////////////
void VisualTest::Material(const std::string &_renderEngine)
{
  RenderEngine *engine = rendering::engine(_renderEngine);
  if (!engine)
  {
    igndbg << "Engine '" << _renderEngine
              << "' is not supported" << std::endl;
    return;
  }

  ScenePtr scene = engine->CreateScene("scene");

  // create visual
  VisualPtr visual = scene->CreateVisual();
  ASSERT_NE(nullptr, visual);

  // check initial material
  EXPECT_EQ(nullptr, visual->Material());

  // create material
  math::Color ambient(0.5, 0.2, 0.4, 1.0);
  math::Color diffuse(0.1, 0.9, 0.3, 1.0);
  math::Color specular(0.8, 0.7, 0.0, 1.0);
  double transparency = 0.3;
  MaterialPtr material = scene->CreateMaterial("unique");
  ASSERT_NE(nullptr, material);
  EXPECT_TRUE(scene->MaterialRegistered("unique"));
  material->SetAmbient(ambient);
  material->SetDiffuse(diffuse);
  material->SetSpecular(specular);
  material->SetTransparency(transparency);

  // Set visual material but do not clone the material
  visual->SetMaterial(material, false);
  EXPECT_EQ(material, visual->Material());

  // Set visual material and clone the material
  visual->SetMaterial(material, true);
  MaterialPtr cloneMat = visual->Material();
  EXPECT_NE(material, cloneMat);
  EXPECT_NE(material->Name(), cloneMat->Name());

  // verify clone material properties
  EXPECT_EQ(ambient, cloneMat->Ambient());
  EXPECT_EQ(diffuse, cloneMat->Diffuse());
  EXPECT_EQ(specular, cloneMat->Specular());
  EXPECT_DOUBLE_EQ(transparency, cloneMat->Transparency());

  // create another material
  math::Color ambient2(0.0, 0.0, 1.0, 1.0);
  math::Color diffuse2(1.0, 0.0, 1.0, 1.0);
  math::Color specular2(0.0, 1.0, 0.0, 1.0);
  double transparency2 = 0;
  MaterialPtr material2 = scene->CreateMaterial("unique2");
  ASSERT_NE(nullptr, material2);
  EXPECT_TRUE(scene->MaterialRegistered("unique2"));
  material2->SetAmbient(ambient2);
  material2->SetDiffuse(diffuse2);
  material2->SetSpecular(specular2);
  material2->SetTransparency(transparency2);

  // Set material to the same visual using its name and verify material changed
  visual->SetMaterial("unique2", true);
  MaterialPtr cloneMat2 = visual->Material();
  EXPECT_NE(material2, cloneMat);
  EXPECT_NE(material2->Name(), cloneMat2->Name());
  EXPECT_EQ(ambient2, cloneMat2->Ambient());
  EXPECT_EQ(diffuse2, cloneMat2->Diffuse());
  EXPECT_EQ(specular2, cloneMat2->Specular());
  EXPECT_DOUBLE_EQ(transparency2, cloneMat2->Transparency());
}

/////////////////////////////////////////////////
TEST_P(VisualTest, VisualProperties)
{
  Material(GetParam());
}

INSTANTIATE_TEST_CASE_P(Visual, VisualTest,
    ::testing::Values("ogre", "optix"));

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}