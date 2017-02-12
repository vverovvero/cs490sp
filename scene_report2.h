
Camera s_camera = {
    .point = {
        .x = 50.0f,
        .y = 50.0f,
        // .z = 01.0f
        .z = 150.0
    },
    .fieldOfView = 60.0f, //60.0f
    .toPoint = {
        .x = 50.0f,
        .y = 25.0f,
        .z = 0.0f
        // .z = 160.0f
    },
    .up = UP
};


////////////////////Lights////////////////
Light s_lights[3] = {
  {
    .type = OMNI,
    .point = {
      .x = 50.0f,
      .y = 95.0f, //95
      .z = 50.0f
    },
    .color = {
      .x = 255.0f,
      .y = 255.0f,
      .z = 255.0f
    },
  },

  {
    .type = OMNI,
    .point = {
      // .x = 5.0f,
      // .y = 95.0f,
      // .z = 100.0f
      .x = 40.0f,
      .y = 95.0f,
      .z = 20.0f
    },
    .color = {
      // .x = 255.0f,
      // .y = 220.0f,
      // .z = 200.0f
      .x = 255.0f,
      .y = 114.0f,
      .z = 168.0f
    },
  },

  {
    .type = OMNI,
    .point = {
      // .x = 95.0f,
      // .y = 5.0f,
      // .z = 100.0f
      .x = 45.0f,
      .y = 05.0f,
      .z = 70.0f
    },
    .color = {
      // .x = 50.0f,
      // .y = 50.0f,
      // .z = 100.0f
      .x = 66.0f,
      .y = 244.0f,
      .z = 98.0f
    },
  }
};

Sphere s_spheres[2] = {

  {
    .point =
      {
        .x = 70.0f,
        .y = 25.0f,
        .z = 50.0f
        // .z = 75.0f
      },
    .radius = 25.0f
  },

  {
    .point =
      {
        .x = 20.0f,
        .y = 10.0f,
        .z = 50.0f
        // .z = 75.0f
      },
    .radius = 10.0f
  },

};

//used to be 10, now 12 with front wall
Triangle s_triangles[12] = {

  //Back wall
  {
    .point1 =
      {
        .x = 0.0f,
        .y = 0.0f,
        .z = 0.0f,
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
        .y = 100.0f,
        .z = 0.0f,
      }
  },

  {
    .point1 =
      {
        .x = 100.0f,
        .y = 0.0f,
        .z = 0.0f,
      },
    .point2 =
      {
        .x = 100.0f,
        .y = 100.0f,
        .z = 0.0f,
      },
    .point3 =
      {
        .x = 0.0f,
        .y = 100.0f,
        .z = 0.0f,
      }
  },

  //FLoor

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
        .z = 160.0f,
      },
    .point3 =
      {
        .x = 100.0f,
        .y = 0.0f,
        .z = 160.0f,
      }
  },

  {
    .point1 =
      {
        .x = 100.0f,
        .y = 0.0f,
        .z = 160.0f,
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
      }
  },

  //Ceiling

  {
    .point1 =
      {
        .x = 0.0f,
        .y = 100.0f,
        .z = 0.0f,
      },
    .point2 =
      {
        .x = 100.0f,
        .y = 100.0f,
        .z = 0.0f,
      },
    .point3 =
      {
        .x = 100.0f,
        .y = 100.0f,
        .z = 160.0f,
      }
  },

  {
    .point1 =
      {
        .x = 100.0f,
        .y = 100.0f,
        .z = 160.0f,
      },
    .point2 =
      {
        .x = 0.0f,
        .y = 100.0f,
        .z = 160.0f,
      },
    .point3 =
      {
        .x = 0.0f,
        .y = 100.0f,
        .z = 0.0f,
      }
  },

  //Left wall red

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
        .y = 100.0f,
        .z = 0.0f,
      },
    .point3 =
      {
        .x = 0.0f,
        .y = 100.0f,
        .z = 160.0f,
      }
  },

  {
    .point1 =
      {
        .x = 0.0f,
        .y = 100.0f,
        .z = 160.0f,
      },
    .point2 =
      {
        .x = 0.0f,
        .y = 0.0f,
        .z = 160.0f,
      },
    .point3 =
      {
        .x = 0.0f,
        .y = 0.0f,
        .z = 0.0f,
      }
  },

  // RIght Wall blue
  {
    .point1 =
      {
        .x = 100.0f,
        .y = 0.0f,
        .z = 0.0f,
      },
    .point2 =
      {
        .x = 100.0f,
        .y = 0.0f,
        .z = 160.0f,
      },
    .point3 =
      {
        .x = 100.0f,
        .y = 100.0f,
        .z = 160.0f,
      }
  },

  {
    .point1 =
      {
        .x = 100.0f,
        .y = 100.0f,
        .z = 160.0f,
      },
    .point2 =
      {
        .x = 100.0f,
        .y = 100.0f,
        .z = 0.0f,
      },
    .point3 =
      {
        .x = 100.0f,
        .y = 0.0f,
        .z = 0.0f,
      }
  },

    //Front wall
  {
    .point1 =
      {
        .x = 100.0f,
        .y = 0.0f,
        .z = 160.0f,
      },
    .point2 =
      {
        .x = 0.0f,
        .y = 0.0f,
        .z = 160.0f,
      },
    .point3 =
      {
        .x = 0.0f,
        .y = 100.0f,
        .z = 160.0f,
      }
  },

  {
    .point3 =
      {
        .x = 100.0f,
        .y = 0.0f,
        .z = 160.0f,
      },
    .point2 =
      {
        .x = 100.0f,
        .y = 100.0f,
        .z = 160.0f,
      },
    .point1 =
      {
        .x = 0.0f,
        .y = 100.0f,
        .z = 160.0f,
      }
  }
};

//originally 12 objects, now 14
Object s_objects[14] = {

  {
    .type = SPHERE,
    .matIndex = 2,
    .object = &(s_spheres[0])
  },

  {
    .type = SPHERE,
    .matIndex = 3,
    .object = &(s_spheres[1])
  },

  // Back wall
  {
    .type = TRIANGLE,
    .matIndex = 1,
    .object = &(s_triangles[0])
  },

  {
    .type = TRIANGLE,
    .matIndex = 1,
    .object = &(s_triangles[1])
  },

  // FLoor
  {
    .type = TRIANGLE,
    .matIndex = 1,
    .object = &(s_triangles[2])
  },

  {
    .type = TRIANGLE,
    .matIndex = 1,
    .object = &(s_triangles[3])
  },

  // Ceiling
  {
    .type = TRIANGLE,
    .matIndex = 1,
    .object = &(s_triangles[4])
  },

  {
    .type = TRIANGLE,
    .matIndex = 1,
    .object = &(s_triangles[5])
  },

  // Left Wall Red
  {
    .type = TRIANGLE,
    .matIndex = 2,
    .object = &(s_triangles[6])
  },

  {
    .type = TRIANGLE,
    .matIndex = 2,
    .object = &(s_triangles[7])
  },

  // Right wall blue
  {
    .type = TRIANGLE,
    .matIndex = 3,
    .object = &(s_triangles[8])
  },

  {
    .type = TRIANGLE,
    .matIndex = 3,
    .object = &(s_triangles[9])
  },

  // Front wall
  {
    .type = TRIANGLE,
    .matIndex = 1,
    .object = &(s_triangles[10])
  },

  {
    .type = TRIANGLE,
    .matIndex = 1,
    .object = &(s_triangles[11])
  }

};

Material s_materials[6] = {

  // Mat 0
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
    .lambert = 0.85f,
    .ambient = 0.05f,
    .exponent = 0.0f,
  },

  // Mat 1 Diff white
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

  // Mat 2 Diff Red
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

  // Mat 3 Diffuse Blue
  {
    .color =
      {
        .x = 90.0f,
        .y = 90.0f,
        .z = 255.0f,
      },
    .type = ORIGINAL,
    .metal = 0,
    .specular = 0.0f,
    .lambert = 0.9f,
    .ambient = 0.1f,
    .exponent = 0.0f,
  },

  // Mat 4 Mirror
  {
    .color =
      {
        .x = 255.0f,
        .y = 255.0f,
        .z = 255.0f,
      },
    .type = ORIGINAL,
    .metal = 0,
    .specular = 0.9f,
    .lambert = 0.1f,
    .ambient = 0.0f,
    .exponent = 0.0f,
  },

  // Mat 5 GOLD!
  {
    .color =
      {
        .x = 200.0f,
        .y = 170.0f,
        .z = 60.0f,
      },
    .type = PHONG,
    .metal = 1,
    .specular = 0.5f,
    .lambert = 0.4f,
    .ambient = 0.1f,
    .exponent = 2.0f,
  }
};

//Originally .n_objects = 12, now 14 with extra wall
Scene s_scene = {
  .camera = &s_camera,
  .materials = s_materials,
  .objects = s_objects,
  .lights = s_lights,
  .n_lights = 3,
  .n_materials = 6,
  .n_objects = 14
};
