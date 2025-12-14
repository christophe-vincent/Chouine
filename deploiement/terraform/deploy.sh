#!/bin/bash

set -e  # Arrêter le script en cas d'erreur

echo "================================================"
echo "Déploiement Terraform - Chouine"
echo "================================================"

# Variables
REGION="eu-west-1"
BUCKET_NAME="jeu-chouine"
STATE_KEY="terraform/state/chouine.tfstate"

# Couleurs pour l'affichage
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Fonction pour afficher les messages
log_info() {
    echo -e "${GREEN}[INFO]${NC} $1"
}

log_warn() {
    echo -e "${YELLOW}[WARN]${NC} $1"
}

log_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Vérifier que Terraform est installé
if ! command -v terraform &> /dev/null; then
    log_error "Terraform n'est pas installé. Veuillez l'installer d'abord."
    exit 1
fi

# Vérifier que AWS CLI est installé
if ! command -v aws &> /dev/null; then
    log_error "AWS CLI n'est pas installé. Veuillez l'installer d'abord."
    exit 1
fi

# Vérifier les credentials AWS
log_info "Vérification des credentials AWS..."
if ! aws sts get-caller-identity &> /dev/null; then
    log_error "Credentials AWS non configurés. Exécutez 'aws configure' d'abord."
    exit 1
fi

log_info "Credentials AWS OK"

# Créer le bucket S3 pour l'état Terraform s'il n'existe pas
log_info "Vérification de l'existence du bucket S3: $BUCKET_NAME"
if ! aws s3 ls "s3://$BUCKET_NAME" 2>&1 | grep -q 'NoSuchBucket'; then
    log_info "Le bucket $BUCKET_NAME existe déjà"
else
    log_error "Le bucket $BUCKET_NAME n'existe pas. Création en cours..."
    exit 1
fi

# Initialiser Terraform avec le backend S3
log_info "Initialisation de Terraform avec backend S3..."
terraform init \
    -backend-config="bucket=$BUCKET_NAME" \
    -backend-config="key=$STATE_KEY" \
    -backend-config="region=$REGION" \
    -backend-config="encrypt=true"

if [ $? -eq 0 ]; then
    log_info "Terraform initialisé avec succès"
else
    log_error "Échec de l'initialisation de Terraform"
    exit 1
fi

# Valider la configuration Terraform
log_info "Validation de la configuration Terraform..."
terraform validate

if [ $? -eq 0 ]; then
    log_info "Configuration valide"
else
    log_error "Configuration invalide"
    exit 1
fi

# Afficher le plan
log_info "Génération du plan Terraform..."
terraform plan -out=tfplan

# Appliquer les changements
log_info "Application du plan Terraform..."
terraform apply tfplan

if [ $? -eq 0 ]; then
    log_info "Déploiement terminé avec succès !"
    rm -f tfplan
    
    echo ""
    echo "================================================"
    log_info "Outputs:"
    terraform output
    echo "================================================"
else
    log_error "Échec du déploiement"
    rm -f tfplan
    exit 1
fi