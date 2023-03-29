using System.Text;
using Application.Interfaces;
using Domain;
using MQTTnet;
using MQTTnet.Client;
using MQTTnet.Extensions.ManagedClient;
using MQTTnet.Packets;

namespace Application;

public class MqttService : IMqttService
{
    private readonly IMqttRepository _repository;
    private readonly ManagedMqttClient _client;
    private CancellationTokenSource AsyncTokenSource { get; } = new ();
    
    public MqttService(IMqttRepository repository)
    {
        _repository = repository ?? throw new ArgumentNullException(nameof(repository));
        
        var factory = new MqttFactory();
        _client = new ManagedMqttClient(factory.CreateMqttClient(), factory.DefaultLogger);
            
        _client.ConnectedAsync += ConnectedAsync;
        _client.DisconnectedAsync += DisconnectedAsync;
        _client.ConnectingFailedAsync += ConnectingFailedAsync;
        _client.ConnectionStateChangedAsync += ConnectionStateChangedAsync;
        _client.ApplicationMessageSkippedAsync += MessageSkippedAsync;
        _client.ApplicationMessageReceivedAsync += MessageReceivedAsync;
        _client.ApplicationMessageProcessedAsync += MessageProcessedAsync;
    }

    public Mqtt create(Mqtt mqtt)
    {
        if (mqtt == null)
        {
            throw new NullReferenceException();
        }
        
        Mqtt returnMqtt = _repository.create(mqtt);
        
        if (returnMqtt == null)
        {
            throw new NullReferenceException();
        }
        
        return returnMqtt;
    }

    public List<Mqtt> readAll()
    {
        List<Mqtt> mqttList = _repository.readAll();
        
        if (mqttList == null)
        {
            throw new NullReferenceException();
        }

        return mqttList;
    }

    public Mqtt read(int id)
    {
        if (id <= 0)
        {
            throw new ArgumentOutOfRangeException();
        }
        
        Mqtt returnMqtt = _repository.read(id);
        
        if (returnMqtt == null)
        {
            throw new NullReferenceException();
        }

        return returnMqtt;
    }

    public Mqtt update(Mqtt mqtt)
    {
        throw new NotImplementedException();
    }

    public bool delete(Mqtt mqtt)
    {
        if (mqtt == null)
        {
            throw new NullReferenceException();
        }
        
        if (mqtt.Id <= 0)
        {
            throw new ArgumentOutOfRangeException();
        }
        
        int returnInt = _repository.delete(mqtt.Id);
        
        if (returnInt == 0)
        {
            return false;
        }
        return true;
    }
    public void Connect()
    {
        Task.Run(async () => {

            if (_client.IsConnected && _client.IsStarted)
                return;

            await _client.StartAsync(new ManagedMqttClientOptionsBuilder()
                .WithAutoReconnectDelay(TimeSpan.FromSeconds(5))
                .WithClientOptions(new MqttClientOptionsBuilder()
                    .WithProtocolVersion(MQTTnet.Formatter.MqttProtocolVersion.V500)
                    .WithClientId("API")
                    .WithConnectionUri("mqtts://mqtt.flespi.io")
                    .WithCredentials("QmV5qNRlM0JpjVNAWYiF1tKp3ialpzx3LiY0WD9yrILAsoYcb7H2ohfindrCDPve")
                    .WithCleanSession()
                    .Build())
                .Build());
        }, AsyncTokenSource.Token);
    }

    public void Disconnect()
    {
        _client.StopAsync().RunSynchronously();

        if (!this.AsyncTokenSource.IsCancellationRequested)
        {
            this.AsyncTokenSource.Cancel();
        }
    }

    public void Publish(string topic, string payload)
    {
        Task.Run(async () => {
            await _client.EnqueueAsync(new MqttApplicationMessageBuilder()
                .WithTopic(topic)
                .WithPayload(payload)
                .WithUserProperty("source", "backend")
                .Build());
        }, AsyncTokenSource.Token);
    }

    public void Subscribe(string topic)
    {
        Task.Run(async () => await _client.SubscribeAsync(topic), AsyncTokenSource.Token);
    }

    public void Unsubscribe(string topic)
    {
        Task.Run(async () => await _client.UnsubscribeAsync(topic), AsyncTokenSource.Token);
    }
    
      protected Task ConnectingFailedAsync(ConnectingFailedEventArgs arg)
        {
            Console.WriteLine($"MQTT client has been disconnected due to: {arg.Exception.Message}");
            return Task.Run(async () => await Task.CompletedTask, AsyncTokenSource.Token);;
        }

        protected Task ConnectionStateChangedAsync(EventArgs arg)
        {
            return Task.Run(async () => await Task.CompletedTask, AsyncTokenSource.Token);;
        }

        protected Task DisconnectedAsync(MqttClientDisconnectedEventArgs arg)
        {
            Console.WriteLine($"MQTT client has been disconnected due to: {arg.Reason.ToString()}"); 
            return Task.Run(async () => await Task.CompletedTask, AsyncTokenSource.Token);;
        }

        protected Task ConnectedAsync(MqttClientConnectedEventArgs arg)
        {
            // GET server settings.
            return Task.Run(async () => await Task.CompletedTask, AsyncTokenSource.Token);;
        }

        protected Task MessageSkippedAsync(ApplicationMessageSkippedEventArgs arg)
        {
            Console.WriteLine($"Skipped message: {arg.ApplicationMessage.Id} - from {arg.ApplicationMessage.ApplicationMessage.Topic}");
            return Task.Run(async () => await Task.CompletedTask, AsyncTokenSource.Token);;
        }

        protected Task MessageProcessedAsync(ApplicationMessageProcessedEventArgs arg)
        {
            return Task.Run(async () => await Task.CompletedTask, AsyncTokenSource.Token);;
        }

        protected Task MessageReceivedAsync(MqttApplicationMessageReceivedEventArgs arg)
        {       
            arg.AutoAcknowledge = false;
            arg.ReasonCode = MqttApplicationMessageReceivedReasonCode.Success;
            arg.ResponseReasonString = "ack";
            arg.ResponseUserProperties.Clear();
            arg.ResponseUserProperties.Add(new MqttUserProperty("source", "backend"));
            Domain.Mqtt msg = new Mqtt();
            msg.Topic = arg.ApplicationMessage.Topic;
            msg.Message = Encoding.UTF8.GetString(arg.ApplicationMessage.Payload);
            msg.Timestamp = DateTime.Now;

            this.create(msg);
            Console.WriteLine($"Received message: {arg.ApplicationMessage.Topic} - {Encoding.UTF8.GetString(arg.ApplicationMessage.Payload)}");
            
            return arg.AcknowledgeAsync(this.AsyncTokenSource.Token);
        }
}