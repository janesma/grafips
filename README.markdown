About **grafips**
==================

**grafips** is a gui front end to fips.  fips instruments the opengl api to provide statistics and metrics for a target application.  some of its features:

* Provides a wide range of gpu metrics for intel hardware.

* Provides cpu/system metrics.

* Allows the user to toggle system settings or opengl overrides, to experiment with various bottlenecks that may be impacting the application:

    * 2x2 Texture replace: minimizes memory bandwidth use due to textures
    * Simple fragment shader: minimizes fragment processing
    * Wireframe mode: helps visualize geometry in the application
    * 1x1 scissor: eliminates geometry processing
    * CPU frequency governor, max/min: change the CPU frequency, to observe impact on GPU/OpenGL metrics

* Optionally saves metrics to a file, for offline analysis or comparison to subsequent collections.

* Connects to a remote target for metrics collection.  This feature insulates the target system from the overhead of running grafips.

See the [grafips wiki](https://github.com/janesma/grafips/wiki) for screenshots and details.


Building **grafips**
======================

grafips has some run-time dependencies:

* Robert Bragg's Intel GPU Performance Counters patches to linux, drm, and mesa.

* fips patches exposing GPU Performance Counters and enabling experiments.

For detailed instructions, refer to the wiki: https://github.com/janesma/grafips/wiki/building

Usage
=====

See information on the wiki:

https://github.com/janesma/grafips/wiki/usage
