using Application.Interfaces;
using Domain;

namespace Application;

public class MqttService : IMqttService
{
    private readonly IMqttRepository _repository;

    public MqttService(IMqttRepository repository)
    {
        _repository = repository ?? throw new ArgumentNullException(nameof(repository));
    }
    
    
}