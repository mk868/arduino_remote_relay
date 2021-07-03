package mk.hardware.test;

import static mk.hardware.test.RelayConsts.IP;
import static mk.hardware.test.RelayConsts.RELAY1;
import static mk.hardware.test.RelayConsts.RELAY2;
import static mk.hardware.test.RelayConsts.RELAY3;
import static mk.hardware.test.RelayConsts.RELAY4;

import feign.Feign;
import feign.gson.GsonDecoder;
import mk.hardware.test.module.ModuleClient;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;

class Example1Test {

  ModuleClient moduleClient;

  @BeforeEach
  void init() {
    moduleClient = Feign.builder()
        .decoder(new GsonDecoder())
        .target(ModuleClient.class, "http://" + IP);

    moduleClient.enable(RELAY1);
    moduleClient.disable(RELAY2);
    moduleClient.disable(RELAY3);
    moduleClient.disable(RELAY4);
  }

  @Test
  void test1() throws Exception {
    // restart device
    moduleClient.disable(RELAY1);
    Thread.sleep(500);
    moduleClient.enable(RELAY1);
    Thread.sleep(5000);

    // TODO check something here
  }

  @Test
  void test2() throws Exception {
    Thread.sleep(1000);

    // simulate plugging via USB to PC
    moduleClient.enable(RELAY2);

    // TODO check something here

    moduleClient.disable(RELAY2);

    // TODO check something here
  }
}
