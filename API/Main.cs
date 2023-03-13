using Application;

namespace API;

partial class Program
{
    // singleton due to the fact that the mqtt client is a stateful object embedded in a blocking async webapi loop.
    static IMqttClient _mqttClient = MqttClientImpl.Instance;

    static void Main(string[] args)
    {
        _mqttClient.Connect();
        _mqttClient.Subscribe("test");

        CreateHostBuilder(args).Build().Run();
    }

    public static IHostBuilder CreateHostBuilder(string[] args)
    {
        return Host.CreateDefaultBuilder(args).ConfigureWebHostDefaults(webBuilder =>
        {
            webBuilder.UseStartup<Startup>();
        });
    }
}


