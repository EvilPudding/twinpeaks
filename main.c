#include <candle.h>
#include <stdio.h>

int main(int argc, char **argv)
{
	/* bl(); exit(1); */

	entity_t model;

	candle = candle_new(0);

	/* entity_add_component(candle->systems, (c_t*)c_renderer_new(1.0f, 0, 0, 1.0f)); */
	entity_add_component(candle->systems, (c_t*)c_renderer_new(1.0f, 1, 1, 1));
	entity_add_component(candle->systems, (c_t*)c_editmode_new());
	c_editmode_activate(c_editmode(&candle->systems));

	/* SKY */
	mesh_t *cube = sauces_mesh("floor.obj");
	mesh_t *cloth = sauces_mesh("cloth.obj");
	entity_new( c_model_new(cube, sauces_mat("pack2/tile"), 1));

	entity_new( c_model_new(cloth, sauces_mat("pack2/velvet"), 1));
	entity_t c1 = entity_new( c_model_new(cloth, sauces_mat("pack2/velvet"), 1));
	entity_t c2 = entity_new( c_model_new(cloth, sauces_mat("pack2/velvet"), 1));
	entity_t c3 = entity_new( c_model_new(cloth, sauces_mat("pack2/velvet"), 1));

	sauces_mat("pack2/tile")->normal.texture_blend = 0.3;

	c_spacial_rotate_Y(c_spacial(&c1), M_PI / 2);
	c_spacial_rotate_Y(c_spacial(&c2), M_PI);
	c_spacial_rotate_Y(c_spacial(&c3), 3 * M_PI / 2);

	mesh_scale_uv(cloth, 10);
	mesh_scale_uv(cube, 10);

	entity_t light = entity_new( c_name_new("light"),
			c_node_new(),
			c_light_new(1.3, vec4(1.0, 1.0, 0.4, 1.0), 512)
	);
	c_spacial_set_pos(c_spacial(&light), vec3(2.0, 2.0, 2.0));

	entity_t ambient = entity_new(c_ambient_new(64));
	c_spacial_set_pos(c_spacial(&ambient), vec3(0, 1, 0));

	entity_t venus = entity_new(
			c_name_new("venus"),
			c_model_new(sauces_mesh("venus.obj"), sauces_mat("pack1/stone3"), 1)
	);
	c_spacial_set_pos(c_spacial(&venus), vec3(0, 0, 0));



	candle_wait(candle);

    return 0;
}
//*/
