using Domain;

namespace Application.Interfaces;

public interface IMqttService
{
    public Mqtt create(Mqtt mqtt);
    public List<Mqtt> readAll();
    public Mqtt read(int id);
    public Mqtt update(Mqtt mqtt);
    public Boolean delete(Mqtt mqtt);
    
    public void Connect();
    public void Disconnect();
    public void Subscribe(string topic);
    public void Unsubscribe(string topic);
    public void Publish(string topic, string payload);
}