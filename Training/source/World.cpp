#include "World.h"
#include "Shader_compiler.h"
#include "Rendering/Shader.h"

void World::init()
{
  
}

void World::render()
{
    for (auto object : mesh_objects)
        object->render();
}

void World::on_lighting_change(Light* light)
{
    std::vector<float> light_pos;
    std::vector<float> light_color;

    for (std::shared_ptr light : lights)
    {
        light_color.push_back(light->get_color().r);
        light_color.push_back(light->get_color().g);
        light_color.push_back(light->get_color().b);
        light_color.push_back(light->get_color().a);

        light_pos.push_back(light->get_location().x);
        light_pos.push_back(light->get_location().y);
        light_pos.push_back(light->get_location().z);
        light_pos.push_back(1.0f);
    }

    auto shaders = Shader_compiler::get_current_shaders();
    for (auto shader : shaders)
    {
        std::shared_ptr shader_ptr = shader.second.lock();
        shader_ptr->set_uniform4fv("lightpos", light_pos.size(), light_pos.data());
        shader_ptr->set_uniform4fv("lightcolor", light_color.size(), light_color.data());
        shader_ptr->set_uniform1i("lightamount", lights.size());
    }
}

glm::mat4 World::get_camera_view_matrix()
{
    if (camera)
        return camera->get_view_matrix();
    else
        return glm::mat4(1);
}

glm::mat4 World::get_camera_projection_matrix()
{
    if (camera)
        return camera->get_projection_matrix();
    else
        return glm::mat4(1);
}

std::shared_ptr<Camera> World::spawn_camera(Transform transform)
{
    this->camera.reset(new Camera(this));
    camera->set_transform(transform);
    return this->camera;
}

std::shared_ptr<Mesh_object> World::spawn_mesh_object(Transform transform)
{
    mesh_objects.emplace_back(new Mesh_object(this));
    mesh_objects.back()->set_transform(transform);
    return mesh_objects.back();
}

std::shared_ptr<Light> World::spawn_light(Transform transform)
{
    lights.emplace_back(new Light(this));
    lights.back()->set_transform(transform);
    return lights.back();
}