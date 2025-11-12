-- Scripts/TestCamera.lua
-- Demonstrates using the Camera bindings from Lua

local cam = nil  -- declare camera at module scope

function OnStart()
    print("TestCamera.lua: OnStart called")

    -- Create a new camera instance
    cam = Camera.create()
    print("Camera created")

    -- Initialize the camera's render texture
    Camera.init_render_texture(cam)
    print("Camera render texture initialized")
end

function OnUpdate()
    -- Each frame, render the scene into the camera's texture
    Camera.render_to_texture(cam)
    print("Camera rendered to texture this frame")
end