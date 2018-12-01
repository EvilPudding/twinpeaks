#include <candle.h>
#include <stdio.h>
#include <components/name.h>
#include <components/decal.h>
#include <components/sprite.h>
#include <components/light.h>
#include <components/model.h>
#include <systems/editmode.h>

int main(int argc, char **argv)
{
	entity_add_component(SYS, (c_t*)c_editmode_new());
	c_editmode_activate(c_editmode(&SYS));

	c_sauces_index_dir(c_sauces(&SYS), "resauces");

	mesh_t *cube = sauces("floor");
	mesh_t *cloth = sauces("cloth");

	entity_new(c_model_new(cube, sauces("tile"), 1, 1));

	entity_t c0 = entity_new(c_model_new(cloth, sauces("velvet"), 1, 1));
	entity_t c1 = entity_new(c_model_new(cloth, sauces("velvet"), 1, 1));
	entity_t c2 = entity_new(c_model_new(cloth, sauces("velvet"), 1, 1));
	entity_t c3 = entity_new(c_model_new(cloth, sauces("velvet"), 1, 1));
	c_model_smooth(c_model(&c0), 1);
	c_model_smooth(c_model(&c1), 1);
	c_model_smooth(c_model(&c2), 1);
	c_model_smooth(c_model(&c3), 1);

	((mat_t*)sauces("tile"))->normal.blend = 0.3;

	c_spatial_rotate_Y(c_spatial(&c1), M_PI / 2);
	c_spatial_rotate_Y(c_spatial(&c2), M_PI);
	c_spatial_rotate_Y(c_spatial(&c3), 3 * M_PI / 2);

	mesh_scale_uv(cube, 10);

	entity_t light = entity_new( c_name_new("light"),
			c_light_new(30.0f, vec4(1.0, 1.0, 0.9, 1.0), 512)
	);
	c_spatial_set_pos(c_spatial(&light), vec3(2.0, 2.0, 2.0));

	entity_t venus = entity_new(
			c_name_new("venus"),
			c_model_new(sauces("venus"), sauces("stone3"), 1, 1)
	);
	c_spatial_set_pos(c_spatial(&venus), vec3(0, 0, 0));

	/* entity_t decal = entity_new(c_decal_new(sauces("piramids"))); */
	/* c_spatial_set_pos(c_spatial(&decal), vec3(1, 0, 1)); */
	/* c_spatial_rotate_Y(c_spatial(&decal), M_PI / 5); */
	/* c_spatial_rotate_X(c_spatial(&decal), -M_PI / 2); */

	entity_t sprite = entity_new(c_sprite_new(sauces("stone3"), 0));
	c_spatial_set_pos(c_spatial(&sprite), vec3(0, 2, 2));

	candle_wait();

    return 0;
}
//*/
