import {
    drag,
    color,
    select,
    range,
    randomUniform,
    scaleOrdinal,
    selectAll,
    schemeCategory10,
  } from "https://cdn.skypack.dev/d3@7.8.5";

  import {
    gridPlanes3D,
    points3D,
    lineStrips3D,
} from "https://cdn.skypack.dev/d3-3d@1.0.0";

document.addEventListener("DOMContentLoaded", () => {
    const origin = { x: 480, y: 250 };
    const startAngle = Math.PI / 4;
    const scale = 20;

    const points3d = points3D()
    .origin(origin)
    .rotateY(startAngle)
    .rotateX(-startAngle)
    .scale(scale);
});