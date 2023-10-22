package mk.hardware.test.module;

import feign.Param;
import feign.RequestLine;
import java.util.List;

public interface ModuleClient {

  @RequestLine("GET /modules")
  List<Module> getModules();

  @RequestLine("GET /modules/{moduleId}")
  Module getModule(@Param("moduleId") String moduleId);

  @RequestLine("GET /modules/{moduleId}?value=1")
  void enable(@Param("moduleId") String moduleId);

  @RequestLine("GET /modules/{moduleId}?value=0")
  void disable(@Param("moduleId") String moduleId);

  @RequestLine("GET /modules/{moduleId}?initialValue=1")
  void enableByDefault(@Param("moduleId") String moduleId);

  @RequestLine("GET /modules/{moduleId}?initialValue=0")
  void disableByDefault(@Param("moduleId") String moduleId);

  @RequestLine("GET /modules/{moduleId}?name={moduleId}")
  void setName(@Param("moduleId") String moduleId, @Param("name") String name);

}
