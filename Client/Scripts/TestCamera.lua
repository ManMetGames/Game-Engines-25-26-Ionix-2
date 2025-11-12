-- Scripts/TestCamera.lua
-- Demonstrates using the Camera bindings from Lua

function OnStart()
    print("TestCamera.lua: OnStart called")

    -- Initialize the camera's render texture
    Camera.init_render_texture(cam)
    print("Camera render texture initialized")
end

function OnUpdate()
    -- Each frame, render the scene into the camera's texture
    Camera.render_to_texture(cam)
    print("Camera rendered to texture this frame")
end