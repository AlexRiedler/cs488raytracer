-- THE POWER IS IN MY HANDS pewny sphere

gold = gr.material({0.9, 0.8, 0.4}, {0.8, 0.8, 0.4}, 50)
grass = gr.material({0.1, 0.7, 0.1}, {1.0, 0.0, 0.0}, 0)
blue = gr.material({0.7, 0.6, 1}, {0.5, 0.4, 0.8}, 10)
red = gr.material({0.7, 0.2, 0.2}, {0.5, 0.4, 0.8}, 1)

scene = gr.node('scene')
scene:translate(6, -2, -15)
scene:rotate('X', 23)

-- the arc
arc = gr.node('arc')
scene:add_child(arc)
arc:rotate('Y', 60)
arc:translate(0,0,-10)
p1 = gr.cube('p1')
arc:add_child(p1)
p1:set_material(gold)
p1:translate(-2.4, 0, -0.4)
p1:scale(0.8, 4, 0.8)

p2 = gr.cube('p2')
arc:add_child(p2)
p2:set_material(gold)
p2:translate(1.6, 0, -0.4)
p2:scale(0.8, 4, 0.8)

s = gr.sphere('s')
arc:add_child(s)
s:set_material(gold)
s:translate(0, 4, 0)
s:scale(4, 0.6, 0.6)

p3 = gr.cube('p3')
arc:add_child(p3)
p3:set_material(blue)
p3:translate(1.6, 4.0, -0.4)
p3:scale(0.8, 3, 0.8)

p3 = gr.cube('p3')
arc:add_child(p3)
p3:set_material(blue)
p3:translate(-2.6, 4.0, -0.4)
p3:scale(0.8, 3, 0.8)

s2 = gr.sphere('s2')
arc:add_child(s2)
s2:set_material(gold)
s2:translate(0, 7, 0)
s2:scale(4, 0.6, 0.6)

s1 = gr.sphere('s')
s1:set_material(blue)
s1:translate(0,3,7)
arc:add_child(s1)

h1 = gr.sphere('h')
h1:set_material(red)
h1:translate(0,8,0)
h1:scale(0.5,0.5,0.5)
arc:add_child(h1)
-- A small stellated dodecahedron.
require('sample/smstdodeca')

steldodec:set_material(red)
steldodec:translate(0,5,-2)
steldodec:scale(0.01,0.01,0.01)
arc:add_child(steldodec)

-- the floor
plane = gr.mesh('plane', {
	{-1, 0, -1},
	{ 1, 0, -1},
	{1,  0, 1},
	{-1, 0, 1},
     }, {
	{3, 2, 1, 0}
     })
scene:add_child(plane)
plane:set_material(grass)
plane:scale(30, 30, 30)

-- The lights
l1 = gr.light({200,200,400}, {0.8, 0.2, 0.2}, {1, 0, 0})
l2 = gr.light({0, 5, -20}, {0.4, 0.4, 0.8}, {1, 0, 0})

gr.render(scene, 'sample.png', 512, 512, 
	  {0, 0, 0,}, {0, 0, -1}, {0, 1, 0}, 50,
	  {0.4, 0.4, 0.4}, {l1, l2})
