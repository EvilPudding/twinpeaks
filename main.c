#include <candle.h>
#include <stdio.h>
#include <components/node.h>
#include <components/force.h>
#include <components/name.h>
#include <components/camera.h>
#include <components/ambient.h>
#include <components/decal.h>
#include <components/sprite.h>
#include <components/light.h>
#include <components/model.h>
#include <systems/renderer.h>
#include <systems/editmode.h>


int main(int argc, char **argv)
{
	entity_add_component(SYS, (c_t*)c_renderer_new(0.66f, 1, 1, 1));
	entity_add_component(SYS, (c_t*)c_editmode_new());
	c_editmode_activate(c_editmode(&SYS));

	mesh_t *cube = sauces_mesh("floor.obj");
	mesh_t *cloth = sauces_mesh("cloth.obj");

	entity_new(c_model_new(cube, sauces_mat("pack2/tile"), 1, 1));

	entity_new(c_model_new(cloth, sauces_mat("pack2/velvet"), 1, 1));
	entity_t c1 = entity_new(c_model_new(cloth, sauces_mat("pack2/velvet"), 1, 1));
	entity_t c2 = entity_new(c_model_new(cloth, sauces_mat("pack2/velvet"), 1, 1));
	entity_t c3 = entity_new(c_model_new(cloth, sauces_mat("pack2/velvet"), 1, 1));

	sauces_mat("pack2/tile")->normal.texture_blend = 0.3;

	c_spacial_rotate_Y(c_spacial(&c1), M_PI / 2);
	c_spacial_rotate_Y(c_spacial(&c2), M_PI);
	c_spacial_rotate_Y(c_spacial(&c3), 3 * M_PI / 2);

	mesh_scale_uv(cube, 10);

	entity_t light = entity_new( c_name_new("light"),
			c_node_new(),
			c_light_new(30.0f, vec4(1.0, 1.0, 0.9, 1.0), 512)
	);
	c_spacial_set_pos(c_spacial(&light), vec3(2.0, 2.0, 2.0));

	entity_t ambient = entity_new(c_ambient_new(64));
	c_spacial_set_pos(c_spacial(&ambient), vec3(0, 1, 0));

	entity_t venus = entity_new(
			c_name_new("venus"),
			c_model_new(sauces_mesh("venus.obj"), sauces_mat("pack1/stone3"), 1, 1)
	);
	c_spacial_set_pos(c_spacial(&venus), vec3(0, 0, 0));

	/* entity_t decal = entity_new(c_decal_new(sauces_mat("pack1/piramids"))); */
	/* c_spacial_set_pos(c_spacial(&decal), vec3(1, 0, 1)); */
	/* c_spacial_rotate_Y(c_spacial(&decal), M_PI / 5); */
	/* c_spacial_rotate_X(c_spacial(&decal), -M_PI / 2); */

	/* entity_t sprite = entity_new(c_sprite_new(sauces_mat("pack1/stone3"), 0)); */
	/* c_spacial_set_pos(c_spacial(&sprite), vec3(0, 2, 2)); */


	candle_wait();

    return 0;
}
//*/
