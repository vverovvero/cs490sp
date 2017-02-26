//simple.h

Camera s_camera = {
    .point = {
        .x = 50.0f,
        .y = 50.0f,
        .z = 400.0
    },
    .fieldOfView = 40.0f,
    .toPoint = {
        .x = 50.0f,
        .y = 50.0f,
        .z = 0.0f
    },
    .up = UP
};

Light s_lights[1] = {
  {
    .type = OMNI,
    .point = {
      .x = 50.0f,
      .y = 95.0f,
      .z = 50.0f
    },
    .color = {
      .x = 155.0f,
      .y = 155.0f,
      .z = 155.0f
    },
  }
};

Material s_materials[2] = {
  {
    .color =
      {
        .x = 255.0f,
        .y = 255.0f,
        .z = 255.0f,
      },
    .type = ORIGINAL,
    .metal = 0,
    .specular = 0.0f,
    .lambert = 0.9f,
    .ambient = 0.05f,
    .exponent = 0.0f,
  },
  {
    .color =
      {
        .x = 255.0f,
        .y = 90.0f,
        .z = 90.0f,
      },
    .type = ORIGINAL,
    .metal = 0,
    .specular = 0.0f,
    .lambert = 0.9f,
    .ambient = 0.1f,
    .exponent = 0.0f,
  },
};

Sphere s_spheres[1] = {
  {
    .point =
      {
        .x = 50.0f,
        .y = 25.0f,
        .z = 50.0f
        // .z = 75.0f
      },
    .radius = 25.0f,
    .matIndex = 1
  }
};

Triangle s_triangles[2] = {
  {
    .point1 =
      {
        .x = 0.0f,
        .y = 0.0f,
        .z = 0.0f,
      },
    .point2 =
      {
        .x = 0.0f,
        .y = 0.0f,
        .z = 100.0f,
      },
    .point3 =
      {
        .x = 100.0f,
        .y = 0.0f,
        .z = 100.0f,
      },
      .matIndex = 0
  },
  {
    .point1 =
      {
        .x = 100.0f,
        .y = 0.0f,
        .z = 100.0f,
      },
    .point2 =
      {
        .x = 100.0f,
        .y = 0.0f,
        .z = 0.0f,
      },
    .point3 =
      {
        .x = 0.0f,
        .y = 0.0f,
        .z = 0.0f,
      },
      .matIndex = 0
  }
};


Object s_objects[3] = {
  {
    .type = SPHERE,
    .matIndex = 1,
    .object = &(s_spheres[0])
  },
  {
    .type = TRIANGLE,
    .matIndex = 0,
    .object = &(s_triangles[0])
  },
  {
    .type = TRIANGLE,
    .matIndex = 0,
    .object = &(s_triangles[1])
  },
};

Scene s_scene = {
  .camera = &s_camera,
  .materials = s_materials,
  .objects = s_objects,
  .lights = s_lights,
  .n_lights = 1,
  .n_materials = 2,
  .n_objects = 3
};
