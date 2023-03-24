using Domain;

namespace Application.Interfaces;

public interface IMqttService
{
    public Mqtt create(Mqtt mqtt);
    public List<Mqtt> readAll();
    public Mqtt read(int id);
    public Mqtt update(Mqtt mqtt);
    public Boolean delete(int id);
    
}