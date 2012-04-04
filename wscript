

def set_options(opt):
  opt.tool_options("compiler_cxx")

def configure(conf):
  conf.check_tool("compiler_cxx")
  conf.check_tool("node_addon")
  conf.check_cfg(package='x11', uselib_store='X11', mandatory=True, args='--cflags --libs')
  conf.check_cfg(package='xcb', uselib_store='XCB', mandatory=True, args='--cflags --libs')
  conf.check_cfg(package='xcb-image', uselib_store='XCB-image', mandatory=True, args='--cflags --libs')

def build(bld):
  obj = bld.new_task_gen("cxx", "shlib", "node_addon")
  obj.uselib = ["X11","XCB","XCB-image"]
  obj.cxxflags = ["-g", "-D_FILE_OFFSET_BITS=64", "-D_LARGEFILE_SOURCE", "-Wall", "-lX11","-lxcb","-lxcb-image"]
  obj.target = "helloworld_eio"
  obj.source = "helloworld_eio.cc"
