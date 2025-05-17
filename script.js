// Mapeamento de cores para status
const STATUS_COLORS = {
    'Partida': 'status-partida',
    'Em voo': 'status-em-voo',
    'Aterrissado': 'status-aterrissado',
    'Cancelado': 'status-cancelado',
    'Atrasado': 'status-atrasado',
    'Embarque': 'status-embarque',
    'Desembarque': 'status-desembarque'
};

// Dados iniciais dos voos
const INITIAL_FLIGHTS = [
    { numero: 1000, companhia: 'ITA', destino: 'Manaus', portao: 1, hora: '21:19', status: 'Atrasado' },
    { numero: 1001, companhia: 'AZUL', destino: 'Fortaleza', portao: 16, hora: '08:20', status: 'Partida' },
    { numero: 1002, companhia: 'VOEPASS', destino: 'Brasilia', portao: 20, hora: '20:19', status: 'Partida' },
    { numero: 1003, companhia: 'VOEPASS', destino: 'Belem', portao: 9, hora: '11:36', status: 'Aterrissado' },
    { numero: 1004, companhia: 'AZUL', destino: 'Belem', portao: 1, hora: '15:32', status: 'Partida' },
    { numero: 1005, companhia: 'AZUL', destino: 'Belem', portao: 14, hora: '04:30', status: 'Aterrissado' },
    { numero: 1006, companhia: 'VOEPASS', destino: 'Curitiba', portao: 7, hora: '10:54', status: 'Aterrissado' },
    { numero: 1007, companhia: 'GOL', destino: 'Salvador', portao: 12, hora: '11:44', status: 'Aterrissado' },
    { numero: 1008, companhia: 'LATAM', destino: 'Belem', portao: 2, hora: '14:32', status: 'Em voo' },
    { numero: 1009, companhia: 'AZUL', destino: 'Porto Alegre', portao: 6, hora: '21:20', status: 'Cancelado' }
];

// Variáveis globais
let flights = [...INITIAL_FLIGHTS];
const MAX_FLIGHTS = 15;

// Atualiza o relógio
function updateTime() {
    document.getElementById('current-time').textContent = new Date().toLocaleTimeString('pt-BR');
}

// Compara horários (formato HH:MM)
function compareTime(t1, t2) {
    const [h1, m1] = t1.split(':').map(Number);
    const [h2, m2] = t2.split(':').map(Number);
    return h1 !== h2 ? h1 - h2 : m1 - m2;
}

// Ordena voos por horário
function sortFlightsByTime(flights) {
    return [...flights].sort((a, b) => compareTime(a.hora, b.hora));
}

// Renderiza a tabela de voos
function renderFlights() {
    const tbody = document.getElementById('flight-table-body');
    const sortedFlights = sortFlightsByTime(flights);
    
    tbody.innerHTML = sortedFlights.length === 0 
        ? '<tr><td colspan="6">Nenhum voo programado</td></tr>'
        : sortedFlights.map(flight => `
            <tr class="flight-row" data-flight-number="${flight.numero}">
                <td>${flight.hora}</td>
                <td>${flight.numero}</td>
                <td>${flight.companhia}</td>
                <td>${flight.destino}</td>
                <td>${flight.portao}</td>
                <td class="status ${STATUS_COLORS[flight.status]}">${flight.status}</td>
            </tr>`).join('');

    // Adiciona eventos de clique nas linhas
    document.querySelectorAll('.flight-row').forEach(row => {
        row.addEventListener('click', () => {
            const flight = flights.find(f => f.numero === parseInt(row.dataset.flightNumber));
            if (flight) showFlightDetail(flight);
        });
    });
}

// Mostra detalhes do voo em um modal
function showFlightDetail(flight) {
    const modal = document.createElement('div');
    modal.className = 'modal';
    modal.innerHTML = `
        <div class="modal-content">
            <button class="close-btn" onclick="this.closest('.modal').remove()">✕</button>
            <div class="modal-header">
                <h2>Voo ${flight.numero}</h2>
            </div>
            <div class="modal-body">
                <div>
                    <p class="text-gray-400 text-sm">Companhia</p>
                    <p>${flight.companhia}</p>
                </div>
                <div>
                    <p class="text-gray-400 text-sm">Destino</p>
                    <p>${flight.destino}</p>
                </div>
                <div>
                    <p class="text-gray-400 text-sm">Hora</p>
                    <p>${flight.hora}</p>
                </div>
                <div>
                    <p class="text-gray-400 text-sm">Portão</p>
                    <p>${flight.portao}</p>
                </div>
                <div>
                    <p class="text-gray-400 text-sm">Status</p>
                    <p class="${STATUS_COLORS[flight.status]}">${flight.status}</p>
                </div>
                <div class="btn-group">
                    <button class="btn btn-primary" onclick="showUpdateStatus(${flight.numero})">
                        Alterar Status
                    </button>
                    <button class="btn btn-danger" onclick="showDeleteConfirmation(${flight.numero})">
                        Excluir
                    </button>
                </div>
            </div>
        </div>`;
    document.body.appendChild(modal);
}

// Mostra formulário para adicionar voo
function showAddFlightForm() {
    const modal = document.createElement('div');
    modal.className = 'modal';
    modal.innerHTML = `
        <div class="modal-content">
            <button class="close-btn" onclick="this.closest('.modal').remove()">✕</button>
            <div class="modal-header">
                <h2>Adicionar Novo Voo</h2>
            </div>
            <div class="modal-body">
                <form onsubmit="handleAddFlight(event)">
                    <div class="form-group">
                        <label class="form-label">Número do Voo</label>
                        <input type="number" id="numero" class="form-input" required min="1">
                    </div>
                    <div class="form-group">
                        <label class="form-label">Companhia</label>
                        <input type="text" id="companhia" class="form-input" required maxlength="20">
                    </div>
                    <div class="form-group">
                        <label class="form-label">Destino</label>
                        <input type="text" id="destino" class="form-input" required maxlength="30">
                    </div>
                    <div class="form-group">
                        <label class="form-label">Portão</label>
                        <input type="number" id="portao" class="form-input" required min="1">
                    </div>
                    <div class="form-group">
                        <label class="form-label">Hora (HH:MM)</label>
                        <input type="text" id="hora" class="form-input" required pattern="^([0-1]?[0-9]|2[0-3]):[0-5][0-9]$">
                    </div>
                    <div class="form-group">
                        <label class="form-label">Status</label>
                        <select id="status" class="form-input" required>
                            ${Object.keys(STATUS_COLORS).map(status => 
                                `<option value="${status}">${status}</option>`
                            ).join('')}
                        </select>
                    </div>
                    <div class="btn-group">
                        <button type="button" class="btn btn-secondary" onclick="this.closest('.modal').remove()">
                            Cancelar
                        </button>
                        <button type="submit" class="btn btn-primary">
                            Adicionar
                        </button>
                    </div>
                </form>
            </div>
        </div>`;
    document.body.appendChild(modal);
}

// Adiciona um novo voo
function handleAddFlight(event) {
    event.preventDefault();
    const form = event.target;
    const newFlight = {
        numero: parseInt(form.numero.value),
        companhia: form.companhia.value,
        destino: form.destino.value,
        portao: parseInt(form.portao.value),
        hora: form.hora.value,
        status: form.status.value
    };

    // Verifica se o número do voo já existe
    if (flights.some(f => f.numero === newFlight.numero)) {
        alert('Número de voo já existe!');
        return;
    }

    flights.push(newFlight);
    
    // Limita o número máximo de voos
    if (flights.length > MAX_FLIGHTS) {
        flights = sortFlightsByTime(flights).slice(0, MAX_FLIGHTS);
    }

    renderFlights();
    form.closest('.modal').remove();
    
    // Destaca o novo voo adicionado
    const row = document.querySelector(`[data-flight-number="${newFlight.numero}"]`);
    if (row) row.classList.add('new');
}

// Mostra formulário para atualizar status
function showUpdateStatus(flightNumber) {
    const flight = flights.find(f => f.numero === flightNumber);
    if (!flight) return;

    const modal = document.createElement('div');
    modal.className = 'modal';
    modal.innerHTML = `
        <div class="modal-content">
            <button class="close-btn" onclick="this.closest('.modal').remove()">✕</button>
            <div class="modal-header">
                <h2>Alterar Status do Voo ${flight.numero}</h2>
            </div>
            <div class="modal-body">
                <div class="mb-4">
                    <p class="text-sm text-gray-400">Destino: ${flight.destino}</p>
                    <p class="text-sm text-gray-400">Hora: ${flight.hora}</p>
                </div>
                <div class="form-group">
                    <label class="form-label">Novo Status</label>
                    <select id="new-status" class="form-input">
                        ${Object.keys(STATUS_COLORS).map(status => 
                            `<option value="${status}" ${flight.status === status ? 'selected' : ''}>${status}</option>`
                        ).join('')}
                    </select>
                </div>
                <div class="btn-group">
                    <button class="btn btn-secondary" onclick="this.closest('.modal').remove()">
                        Cancelar
                    </button>
                    <button class="btn btn-primary" onclick="handleUpdateStatus(${flight.numero})">
                        Confirmar
                    </button>
                </div>
            </div>
        </div>`;
    document.body.appendChild(modal);
}

// Atualiza o status de um voo
function handleUpdateStatus(flightNumber) {
    const flight = flights.find(f => f.numero === flightNumber);
    if (flight) {
        flight.status = document.getElementById('new-status').value;
        renderFlights();
        document.querySelector('.modal').remove();
    }
}

// Mostra confirmação para excluir voo
function showDeleteConfirmation(flightNumber) {
    const flight = flights.find(f => f.numero === flightNumber);
    if (!flight) return;

    const modal = document.createElement('div');
    modal.className = 'modal';
    modal.innerHTML = `
        <div class="modal-content">
            <button class="close-btn" onclick="this.closest('.modal').remove()">✕</button>
            <div class="modal-body text-center">
                <svg class="mx-auto mb-4" width="48" height="48" viewBox="0 0 24 24" fill="none" stroke="#ef4444" stroke-width="2" stroke-linecap="round" stroke-linejoin="round">
                    <path d="M10.29 3.86L1.82 18a2 2 0 0 0 1.71 3h16.94a2 2 0 0 0 1.71-3L13.71 3.86a2 2 0 0 0-3.42 0z"/>
                    <line x1="12" y1="9" x2="12" y2="13"/>
                    <line x1="12" y1="17" x2="12.01" y2="17"/>
                </svg>
                <h2 class="text-xl font-bold mb-2">Confirmar Exclusão</h2>
                <p class="mb-6">
                    Tem certeza que deseja excluir o voo ${flight.numero} para ${flight.destino}?
                </p>
                <div class="btn-group">
                    <button class="btn btn-secondary" onclick="this.closest('.modal').remove()">
                        Cancelar
                    </button>
                    <button class="btn btn-danger" onclick="handleDeleteFlight(${flight.numero})">
                        Excluir
                    </button>
                </div>
            </div>
        </div>`;
    document.body.appendChild(modal);
}

// Exclui um voo
function handleDeleteFlight(flightNumber) {
    flights = flights.filter(f => f.numero !== flightNumber);
    renderFlights();
    document.querySelector('.modal').remove();
}

// Inicialização da aplicação
document.addEventListener('DOMContentLoaded', () => {
    // Define o ano atual no footer
    document.getElementById('current-year').textContent = new Date().getFullYear();
    
    // Atualiza o relógio a cada segundo
    updateTime();
    setInterval(updateTime, 1000);
    
    // Configura o botão de adicionar voo
    document.getElementById('add-flight-btn').addEventListener('click', showAddFlightForm);
    
    // Renderiza os voos iniciais
    renderFlights();
});