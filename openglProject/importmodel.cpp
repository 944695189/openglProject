#include "importModel.h"

void ImportModel::initShader(const char* vsfile, const char* fsfile)
{
	ourShader.shader(vsfile, fsfile);
	
	
}
void ImportModel::initModel(const char* path) 
{
	ourModel.model(path);
}
void ImportModel::render()
{
	ourShader.use();

	model = glm::translate(mat4(1.0f), glm::vec3(3.0f, -3.0f, .0f)); 
	model = glm::scale(model, glm::vec3(.5f, .5f, .5f));	
	//model = glm::rotate(model, float(10.5f), glm::vec3(1.0, .0, .0));
	//model2 *= glm::rotate(mat4(1.0f), float(.2f), glm::vec3(.0, 1.0, .0));
	//model = model1 * model2;
	ourShader.setUniform("ModelMatrix", model);
	ourShader.setUniform("ProjectionMatrix", projection);
	ourShader.setUniform("ViewMatrix", view);

	ourShader.setUniform("lightColor", lightColor);
	ourShader.setUniform("lightPos", lightPosition);
	ourShader.setUniform("viewPos", viewPosition);
	ourShader.setUniform("Ka", Ka);
	ourShader.setUniform("Kd", Kd);
	ourShader.setUniform("Ks", Ks);
	ourShader.setUniform("shiness", shiness);
	ourModel.draw(ourShader);
}