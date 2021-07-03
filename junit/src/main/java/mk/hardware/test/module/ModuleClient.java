package mk.hardware.test.module;

import feign.Param;
import feign.RequestLine;
import java.util.List;

public interface ModuleClient {

  @RequestLine("GET /modules")
  List<Module> getModules();

  @RequestLine("GET /modules/{module}")
  Module getModule(@Param("module") String module);

  @RequestLine("GET /modules/{module}?value=1")
  void enable(@Param("module") String module);

  @RequestLine("GET /modules/{module}?value=0")
  void disable(@Param("module") String module);

}
