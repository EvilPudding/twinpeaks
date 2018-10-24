#include <candle.h>
#include <stdio.h>
#include <components/node.h>
#include <components/name.h>
#include <components/camera.h>
#include <components/ambient.h>
#include <components/decal.h>
#include <components/sprite.h>
#include <components/light.h>
#include <components/model.h>
#include <utils/renderer.h>
#include <systems/editmode.h>

entity_t load_model(entity_t root, int argc, char **argv)
{
	if(argc <= 1) return entity_null;

	float scale = 1.0f;
	if(argc >= 2)
	{
		if(sscanf(argv[2], "%f", &scale) == -1) scale = 1.0f;
	}
	entity_t ent = c_editmode(&SYS)->selected;
	/* ent = sauces_model("toyota_ae86_drift", 1.0f); */
	/* ent = sauces_model("cubes", 1.0f); */
	/* ent = sauces_model("ass", 1.0f); */
	/* sauces_model(&ent, argv[1], scale); */
	struct load_signal ev = {argv[1], scale};
	entity_signal(entity_null, ref("load"), &ev, &ent);
	return ent;
}

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

	c_spacial_rotate_Y(c_spacial(&c1), M_PI / 2);
	c_spacial_rotate_Y(c_spacial(&c2), M_PI);
	c_spacial_rotate_Y(c_spacial(&c3), 3 * M_PI / 2);

	mesh_scale_uv(cube, 10);

	entity_t light = entity_new( c_name_new("light"),
			c_light_new(30.0f, vec4(1.0, 1.0, 0.9, 1.0), 512)
	);
	c_spacial_set_pos(c_spacial(&light), vec3(2.0, 2.0, 2.0));

	entity_t venus = entity_new(
			c_name_new("venus"),
			c_model_new(sauces("venus"), sauces("stone3"), 1, 1)
	);
	c_spacial_set_pos(c_spacial(&venus), vec3(0, 0, 0));

	/* entity_t decal = entity_new(c_decal_new(sauces("piramids"))); */
	/* c_spacial_set_pos(c_spacial(&decal), vec3(1, 0, 1)); */
	/* c_spacial_rotate_Y(c_spacial(&decal), M_PI / 5); */
	/* c_spacial_rotate_X(c_spacial(&decal), -M_PI / 2); */

	entity_t sprite = entity_new(c_sprite_new(sauces("stone3"), 0));
	c_spacial_set_pos(c_spacial(&sprite), vec3(0, 2, 2));

	candle_reg_cmd("load", (cmd_cb)load_model);

	candle_wait();

    return 0;
}
//*/
