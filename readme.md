# Vulkan

This repo documents my process going through the [Vulkan Tutorial](https://vulkan-tutorial.com/) website.

I plan to use this to reach a point in:

* understanding Vulkan and how to use it
* additional comments and details
* additional experiments and features I want to try out in Vulkan

## Future Goals

I am working on games/game rendering tech, and I want to have a solid understanding of Vulkan before I 
try to abstract it's features.

Process:

- [x] setup
- [x] instance
- [x] validation and debugging
- [x] physical devices and queues
- [x] logical devices and queues
- [x] window surface and present queue
- [x] swapchain
- [x] image views
- [x] shaders
- [x] fixed functions
- [x] render passes
- [x] finished graphics pipeline
- [x] framebuffers
- [x] command buffers
- [x] rendering and presentation

At this point, we've got a rendered triangle on the screen:

![triangle](/assets/triangle_2021-02-09.png)

- [x] swapchain recreation
- [x] vertex input
- [x] vertex buffers
- [x] staging buffers
- [x] index buffers
- [x] descriptor data
- [x] descriptor management
- [x] images
- [x] image view & samplers
- [x] combined image samplers
- [x] depth buffering
- [x] models
- [x] mipmaps
- [x] multisampling

Now we have mip-maps, meshes, and multisampling support:

![mesh_msaa_mipped](/assets/mesh_msaa_mipped_2021-02-15.png)

Future Topics:

- [x] push constants
- [ ] instanced rendering
- [ ] dynamic uniforms
- [ ] pipeline cache
- [ ] allocator
- [ ] multi-resource (texture, mesh, etc.)
- [ ] multi-threaded
- [ ] multiple subpasses
- [ ] compute shaders
- [ ] utils library (aggregate vkObjects into classes)
- [ ] renderer (abstract vulkan from renderer interface)