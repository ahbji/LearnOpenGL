你能像我们教程一开始那样根据一些材质的属性来模拟一个真实世界的物体吗？ 注意材质表中的环境光颜色与漫反射光的颜色可能不一样，因为他们并没有把光照强度考虑进去来模拟，你需要将光照颜色的强度改为vec(1.0f)来输出正确的结果：参考解答，我做了一个青色(Cyan)的塑料箱子

// 设置 cyan plastic 材质
// 参考 http://devernay.free.fr/cours/opengl/materials.html
lightingShader.setVec3("material.ambient", 0.0f, 0.1f, 0.06f);
lightingShader.setVec3("material.diffuse", 0.0f, 0.50980392f, 0.50980392f);
lightingShader.setVec3("material.specular", 0.50196078f, 0.50196078f, 0.50196078f);
lightingShader.setFloat("material.shininess", 0.25f * 128.0f);