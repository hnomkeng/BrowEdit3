#include <Windows.h>
#include "Rsw.h"
#include <browedit/BrowEdit.h>
#include <browedit/Node.h>
#include <browedit/components/RsmRenderer.h>
#include <browedit/components/BillboardRenderer.h>
#include <browedit/util/FileIO.h>

#include <iostream>
#include <fstream>
#include <json.hpp>
#include <glm/gtc/type_ptr.hpp>




RswObject::RswObject(RswObject* other) : position(other->position), rotation(other->rotation), scale(other->scale)
{
}

void RswObject::load(std::istream* is, int version, bool loadModel)
{
	int type;
	is->read(reinterpret_cast<char*>(&type), sizeof(int));
	if (type == 1)
	{
		node->addComponent(new RswModel());
		node->addComponent(new RswModelCollider());
		node->getComponent<RswModel>()->load(is, version, loadModel);
	}
	else if (type == 2)
	{
		node->addComponent(new RswLight());
		node->getComponent<RswLight>()->load(is);
		node->addComponent(new BillboardRenderer("data\\light.png", "data\\light_selected.png"));
		node->addComponent(new CubeCollider(5));
	}
	else if (type == 3)
	{
		node->addComponent(new RswSound());
		node->getComponent<RswSound>()->load(is, version);
		node->addComponent(new BillboardRenderer("data\\sound.png", "data\\sound_selected.png"));
		node->addComponent(new CubeCollider(5));
	}
	else if (type == 4)
	{
		node->addComponent(new RswEffect());
		node->getComponent<RswEffect>()->load(is);
		node->addComponent(new BillboardRenderer("data\\effect.png", "data\\effect_selected.png"));
		node->addComponent(new CubeCollider(5));
	}
	else
		std::cerr << "RSW: Error loading object in RSW, objectType=" << type << std::endl;


}





void RswObject::save(std::ofstream& file, int version)
{
	RswModel* rswModel = nullptr;
	RswEffect* rswEffect = nullptr;
	RswLight* rswLight = nullptr;
	RswSound* rswSound = nullptr;
	int type = 0;
	if (rswModel = node->getComponent<RswModel>())
		type = 1;
	if (rswLight = node->getComponent<RswLight>())
		type = 2;
	if (rswSound = node->getComponent<RswSound>())
		type = 3;
	if (rswEffect = node->getComponent<RswEffect>())
		type = 4;
	file.write(reinterpret_cast<char*>(&type), sizeof(int));
	if (rswModel)		rswModel->save(file, version); //meh don't like this if...maybe make this an interface savable
	if (rswEffect)		rswEffect->save(file);
	if (rswLight)		rswLight->save(file);
	if (rswSound)		rswSound->save(file, version);
}






void RswObject::buildImGui(BrowEdit* browEdit)
{
	auto renderer = node->getComponent<RsmRenderer>();
	ImGui::Text("Object");

	if (util::DragFloat3(browEdit, browEdit->activeMapView->map, node, "Position", &position, 1.0f, 0.0f, 0.0f, "Moving") && renderer)
		renderer->setDirty();
	if (util::DragFloat3(browEdit, browEdit->activeMapView->map, node, "Scale", &scale, 1.0f, 0.0f, 0.0f, "Resizing") && renderer)
		renderer->setDirty();
	if (util::DragFloat3(browEdit, browEdit->activeMapView->map, node, "Rotation", &rotation, 1.0f, 0.0f, 0.0f, "Rotating") && renderer)
		renderer->setDirty();
}
