添加一个叫做放射光贴图(Emission Map)的东西，即记录每个片段发光值(Emission Value)大小的贴图，发光值是(模拟)物体自身发光(Emit)时可能产生的颜色。这样的话物体就可以忽略环境光自身发光。通常在你看到游戏里某个东西(比如 机器人的眼,或是箱子上的小灯)在发光时，使用的就是放射光贴图。使用这个贴图(作者为 creativesam)作为放射光贴图并使用在箱子上，你就会看到箱子上有会发光的字了。参考解答,片段着色器, 最终效果

unsigned int emissionMap = loadTexture(FileSystem::getPath("resources/textures/matrix.jpg").c_str());

lightingShader.setInt("material.emission", 2);

glActiveTexture(GL_TEXTURE2);
glBindTexture(GL_TEXTURE_2D, emissionMap);